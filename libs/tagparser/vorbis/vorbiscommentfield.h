#ifndef TAG_PARSER_VORBISCOMMENTFIELD_H
#define TAG_PARSER_VORBISCOMMENTFIELD_H

#include "../generictagfield.h"

namespace CppUtilities {
class BinaryReader;
class BinaryWriter;
} // namespace CppUtilities

namespace TagParser {

/*!
 * \brief The VorbisCommentFlags enum specifies flags which controls parsing and making of Vorbis comments.
 */
enum class VorbisCommentFlags : std::uint8_t {
    None = 0x0, /**< Regular parsing/making. */
    NoSignature = 0x1, /**< Skips the signature when parsing and making. */
    NoFramingByte = 0x2, /**< Doesn't expect the framing bit to be present when parsing; does not make the framing bit when making. */
    NoCovers = 0x4 /**< Skips all covers when making. */
};

constexpr bool operator&(VorbisCommentFlags lhs, VorbisCommentFlags rhs)
{
    return static_cast<std::uint8_t>(lhs) & static_cast<std::uint8_t>(rhs);
}

constexpr VorbisCommentFlags operator|(VorbisCommentFlags lhs, VorbisCommentFlags rhs)
{
    return static_cast<VorbisCommentFlags>(static_cast<std::uint8_t>(lhs) | static_cast<std::uint8_t>(rhs));
}

class VorbisCommentField;
class Diagnostics;

/*!
 * \brief Defines traits for the TagField implementation of the VorbisCommentField class.
 */
template <> class TAG_PARSER_EXPORT TagFieldTraits<VorbisCommentField> {
public:
    using IdentifierType = std::string;
    using TypeInfoType = std::uint32_t;
};

class OggIterator;

class TAG_PARSER_EXPORT VorbisCommentField : public TagField<VorbisCommentField> {
    friend class TagField<VorbisCommentField>;

public:
    VorbisCommentField();
    VorbisCommentField(const IdentifierType &id, const TagValue &value);

    void parse(OggIterator &iterator, Diagnostics &diag);
    void parse(OggIterator &iterator, std::uint64_t &maxSize, Diagnostics &diag);
    void parse(std::istream &stream, std::uint64_t &maxSize, Diagnostics &diag);
    bool make(CppUtilities::BinaryWriter &writer, VorbisCommentFlags flags, Diagnostics &diag);
    bool isAdditionalTypeInfoUsed() const;
    bool supportsNestedFields() const;

    static typename std::string fieldIdFromString(const char *idString, std::size_t idStringSize = std::string::npos);
    static std::string fieldIdToString(const std::string &id);

private:
    void reset();
    template <class StreamType> void internalParse(StreamType &stream, std::uint64_t &maxSize, Diagnostics &diag);
};

/*!
 * \brief Returns whether the additional type info is used.
 */
inline bool VorbisCommentField::isAdditionalTypeInfoUsed() const
{
    return false;
}

/*!
 * \brief Returns whether nested fields are supported.
 */
inline bool VorbisCommentField::supportsNestedFields() const
{
    return false;
}

/*!
 * \brief Converts the specified ID string representation to an actual ID.
 * \remarks As Vorbis field IDs are plain text the string is just passed.
 */
inline std::string VorbisCommentField::fieldIdFromString(const char *idString, std::size_t idStringSize)
{
    return idStringSize != std::string::npos ? std::string(idString, idStringSize) : std::string(idString);
}

/*!
 * \brief Returns the string representation for the specified \a id.
 * \remarks As Vorbis field IDs are plain text the string is just passed.
 */
inline std::string VorbisCommentField::fieldIdToString(const std::string &id)
{
    return id;
}

/*!
 * \brief Resets Vorbis Comment-specific values. Called via clear().
 */
inline void VorbisCommentField::reset()
{
}

} // namespace TagParser

#endif // TAG_PARSER_VORBISCOMMENTFIELD_H
