#ifndef TAG_PARSER_MATROSKACONTAINER_H
#define TAG_PARSER_MATROSKACONTAINER_H

#include "./ebmlelement.h"
#include "./matroskaattachment.h"
#include "./matroskachapter.h"
#include "./matroskatag.h"
#include "./matroskatrack.h"

#include "../genericcontainer.h"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace TagParser {

class MatroskaSeekInfo;
class MatroskaEditionEntry;

class MediaFileInfo;

class TAG_PARSER_EXPORT MatroskaContainer : public GenericContainer<MediaFileInfo, MatroskaTag, MatroskaTrack, EbmlElement> {
public:
    MatroskaContainer(MediaFileInfo &stream, std::uint64_t startOffset);
    ~MatroskaContainer() override;

    void validateIndex(Diagnostics &diag);
    std::uint64_t maxIdLength() const;
    std::uint64_t maxSizeLength() const;
    const std::vector<std::unique_ptr<MatroskaSeekInfo>> &seekInfos() const;

    static std::uint64_t maxFullParseSize();
    void setMaxFullParseSize(std::uint64_t maxFullParseSize);
    const std::vector<std::unique_ptr<MatroskaEditionEntry>> &editionEntires() const;
    MatroskaChapter *chapter(std::size_t index) override;
    std::size_t chapterCount() const override;
    MatroskaAttachment *createAttachment() override;
    MatroskaAttachment *attachment(std::size_t index) override;
    std::size_t attachmentCount() const override;
    ElementPosition determineElementPosition(std::uint64_t elementId, Diagnostics &diag) const;
    ElementPosition determineTagPosition(Diagnostics &diag) const override;
    ElementPosition determineIndexPosition(Diagnostics &diag) const override;

    virtual bool supportsTitle() const override;
    virtual std::size_t segmentCount() const override;

    void reset() override;

protected:
    void internalParseHeader(Diagnostics &diag) override;
    void internalParseTags(Diagnostics &diag) override;
    void internalParseTracks(Diagnostics &diag) override;
    void internalParseChapters(Diagnostics &diag) override;
    void internalParseAttachments(Diagnostics &diag) override;
    void internalMakeFile(Diagnostics &diag, AbortableProgressFeedback &progress) override;

private:
    void parseSegmentInfo(Diagnostics &diag);
    void readTrackStatisticsFromTags(Diagnostics &diag);

    std::uint64_t m_maxIdLength;
    std::uint64_t m_maxSizeLength;

    std::vector<EbmlElement *> m_tracksElements;
    std::vector<EbmlElement *> m_segmentInfoElements;
    std::vector<EbmlElement *> m_tagsElements;
    std::vector<EbmlElement *> m_chaptersElements;
    std::vector<EbmlElement *> m_attachmentsElements;
    std::vector<std::unique_ptr<MatroskaSeekInfo>> m_seekInfos;
    std::vector<std::unique_ptr<MatroskaEditionEntry>> m_editionEntries;
    std::vector<std::unique_ptr<MatroskaAttachment>> m_attachments;
    std::size_t m_segmentCount;
    static std::uint64_t m_maxFullParseSize;
};

/*!
 * \brief Returns the maximal ID length in bytes.
 */
inline std::uint64_t MatroskaContainer::maxIdLength() const
{
    return m_maxIdLength;
}

/*!
 * \brief Returns the maximal size length in bytes.
 */
inline std::uint64_t MatroskaContainer::maxSizeLength() const
{
    return m_maxSizeLength;
}

/*!
 * \brief Returns seek information read from "SeekHead"-elements when parsing segment info.
 */
inline const std::vector<std::unique_ptr<MatroskaSeekInfo>> &MatroskaContainer::seekInfos() const
{
    return m_seekInfos;
}

/*!
 * \brief Returns the maximal file size for a "full parse" in byte.
 *
 * The "Tags" element (which holds the tag information) is commonly at the end of a Matroska file. Hence the
 * parser needs to walk through the entire file to find the tag information if no "SeekHead" element is present
 * which might causes long loading times. To avoid this a maximal file size for a "full parse" can be specified.
 * The disadvantage is that the parser relies on the presence of a SeekHead element on larger files to retrieve
 * tag information.
 *
 * The default value is 50 MiB.
 *
 * \sa setMaxFullParseSize()
 */
inline std::uint64_t MatroskaContainer::maxFullParseSize()
{
    return m_maxFullParseSize;
}

/*!
 * \brief Sets the maximal file size for a "full parse" in byte.
 * \sa maxFullParseSize()
 */
inline void MatroskaContainer::setMaxFullParseSize(std::uint64_t maxFullParseSize)
{
    m_maxFullParseSize = maxFullParseSize;
}

/*!
 * \brief Returns the edition entries.
 */
inline const std::vector<std::unique_ptr<MatroskaEditionEntry>> &MatroskaContainer::editionEntires() const
{
    return m_editionEntries;
}

inline MatroskaAttachment *MatroskaContainer::attachment(std::size_t index)
{
    return m_attachments.at(index).get();
}

inline std::size_t MatroskaContainer::attachmentCount() const
{
    return m_attachments.size();
}

inline bool MatroskaContainer::supportsTitle() const
{
    return true;
}

inline std::size_t MatroskaContainer::segmentCount() const
{
    return m_segmentInfoElements.size();
}

} // namespace TagParser

#endif // MATROSKACONTAINER_H
