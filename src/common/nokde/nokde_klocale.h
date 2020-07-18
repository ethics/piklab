#ifndef NOKDE_KLOCALE_H
#define NOKDE_KLOCALE_H

#define KDECORE_EXPORT
#define KDE_DEPRECATED
#define KDECORE_EXPORT_DEPRECATED

#include <memory>

#include <qtextstream.h>
#include <qstring.h>
#include <qurl.h>

class KLocale
{
public:
  void setActiveCatalog(const char*) {}
};

class KLocalizedString : public QString
{
public:
  KLocalizedString(const QString& s = QString::null) : QString(s) {}
  QString toString(int = 0) const { return *this; }
  QString toString(KLocale*) const { return *this; }
};

class KAboutData;

class KComponentData
{
public:
  KComponentData(const KAboutData*) {}
  void config() {}
};

inline const char* I18N_NOOP(const char* text) { return text; }
inline QString i18n(const QString &text) { return text; }
template <typename A1>
  QString i18n(const QString &text, const A1& a1) { return text.arg(a1); }
template <typename A1, typename A2>
  QString i18n(const QString &text, const A1& a1, const A2& a2) { return text.arg(a1).arg(a2); }
inline KLocalizedString ki18n(const QString &text) { return text; }
inline QString i18nc(const QString &, const QString &text) { return text; }
template<typename A1>
  QString i18nc(const QString &, const QString &text, const A1& a1) { return text.arg(a1); }
template<typename A1, typename A2>
  QString i18nc(const QString &, const QString &text, const A1& a1, const A2& a2) { return text.arg(a1).arg(a2); }
template<typename A1, typename A2, typename A3>
  QString i18nc(const QString &, const QString &text, const A1& a1, const A2& a2, const A3& a3) { return text.arg(a1).arg(a2).arg(a3); }
template<typename A1, typename A2, typename A3, typename A4>
  QString i18nc(const QString &, const QString &text, const A1& a1, const A2& a2, const A3& a3, const A4& a4) { return text.arg(a1).arg(a2).arg(a3).arg(a4); }
inline KLocalizedString ki18nc(const QString &, const QString &text) { return text; }
inline QString i18n(const QString &, const QString &text) { return text; }

namespace KStandardDirs
{
  inline QString locate(const QString &, const QString &) { return QString::null; }
}

namespace KGlobal
{
  extern QTextStream outStream;
  inline KLocale* locale() { return NULL; }
  inline bool hasLocale() { return false; }
  inline bool hasMainComponent() { return false; }
}

namespace KuitSemantics {
  inline QString escape(const QString& s) { return s; }
}

template <typename T>
class GlobalStatic : public std::auto_ptr<T>
{
public:
    GlobalStatic() : std::auto_ptr<T>(new T) {}
    bool isDestroyed() const { return std::auto_ptr<T>::get() == NULL; }
};

namespace KShell
{
  inline QString tildeExpand(const QString& path) { return path; }
}

#define K_GLOBAL_STATIC(type, name) static GlobalStatic<type> name;
#define KDE_VERSION_STRING "0.0.0"
#define kWarning() KGlobal::outStream

#endif
