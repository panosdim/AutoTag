#ifndef APPLICATION_UTILITIES_FAKEQTCONFIGARGUMENTS_H
#define APPLICATION_UTILITIES_FAKEQTCONFIGARGUMENTS_H

#include "./argumentparser.h"

namespace CppUtilities {

class CPP_UTILITIES_EXPORT FakeQtConfigArguments {
public:
    FakeQtConfigArguments();

    Argument &qtWidgetsGuiArg();
    Argument &qtQuickGuiArg();

    bool areQtGuiArgsPresent() const;

private:
    Argument m_qtWidgetsGuiArg;
    Argument m_qtQuickGuiArg;
};

/*!
 * \brief Returns the argument to show the Qt-widgets-based GUI.
 */
inline Argument &FakeQtConfigArguments::qtWidgetsGuiArg()
{
    return m_qtWidgetsGuiArg;
}

/*!
 * \brief Returns the argument to show the Qt-quick-based GUI.
 */
inline Argument &FakeQtConfigArguments::qtQuickGuiArg()
{
    return m_qtQuickGuiArg;
}

/*!
 * \brief Returns whether at least one of the arguments is present.
 */
inline bool FakeQtConfigArguments::areQtGuiArgsPresent() const
{
    return m_qtWidgetsGuiArg.isPresent() || m_qtQuickGuiArg.isPresent();
}

} // namespace CppUtilities

#ifndef QT_CONFIG_ARGUMENTS
#define QT_CONFIG_ARGUMENTS CppUtilities::FakeQtConfigArguments
#endif

#endif // APPLICATION_UTILITIES_FAKEQTCONFIGARGUMENTS_H
