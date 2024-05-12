/**
 * Copyleft (C) 2023 maminjie <canpool@163.com>
 **/
#pragma once

#include "officeframehelper.h"
#include "ribbon_global.h"
#include <QObject>
#include <QRect>
#include <QSize>
#include <QTimer>
#include <qt_windows.h>
#include <shobjidl.h>

QRIBBON_BEGIN_NAMESPACE

class StyleOptionFrame;
class RibbonBar;

class OfficeFrameHelperWin : public QObject, public OfficeFrameHelper
{
    Q_OBJECT
public:
    OfficeFrameHelperWin(QWidget *parent = Q_NULL);
    virtual ~OfficeFrameHelperWin();

    static UINT m_msgGetFrameHook;
public:
    virtual void enableOfficeFrame(QWidget *parent);
    void disableOfficeFrame();

    virtual bool isDwmEnabled() const;
    virtual bool isActive() const;
    virtual bool isMaximize() const;
    virtual bool canHideTitle() const;

    virtual int frameBorder() const;
    virtual int titleBarSize() const;
    virtual int tabBarHeight() const;
    virtual bool clientMetrics(int &iCaptionWidth, int &iCaptionHeight) const;

    virtual void setHeaderRect(const QRect &rcHeader);
    virtual void setContextHeader(QList<ContextHeader *> *listContextHeaders);

    virtual QPixmap getFrameSmallIcon() const;
    virtual QString getSystemMenuString(uint item) const;
    virtual QString getWindowText() const;

    virtual void drawTitleBar(QPainter *painter, const StyleOptionTitleBar &opt);
    virtual void fillSolidRect(QPainter *painter, const QRect &rect, const QRegion &airRegion, QColor clr);
    virtual void drawDwmCaptionText(QPainter *painter, const QRect &rect, const QString &strWindowText,
                                    const QColor &colText, bool active, bool contextTab);
    virtual QSize sizeSystemIcon(const QIcon &icon, const QRect &rect) const;
    virtual HICON windowIcon(const QIcon &icon, const QRect &rect) const;
    virtual void resetWindowIcon();
    virtual void recalcFrameLayout();
    virtual void postRecalcFrameLayout();
    virtual void processClickedSubControl(QStyle::SubControl subControl);
public:
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    void collapseTopFrame();
#endif   // QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    virtual bool winEvent(MSG *message, long *result);
private:
    static int glowSize() { return 10; }
    static int frameSize() { return GetSystemMetrics(SM_CYSIZEFRAME); }
    static int captionSize() { return GetSystemMetrics(SM_CYCAPTION); }
protected:
    DWORD getStyle(bool exStyle = false) const;
    void refreshFrameStyle();
    void updateFrameRegion();
    void updateFrameRegion(const QSize &szFrameRegion, bool bUpdate);
    void updateShellAutohideBars();
    int shellAutohideBars();

    bool isMDIMaximized() const;
    bool isTitleVisible() const;
    void initStyleOption(StyleOptionFrame *option);
    void redrawFrame();
    bool modifyStyle(HWND hWnd, DWORD dwRemove, DWORD dwAdd, UINT nFlags);
    bool isFrameHasStatusBar(int *statusHeight = Q_NULL) const;
    HRGN calcFrameRegion(QSize sz);
    HFONT getCaptionFont(HANDLE hTheme);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    HDC backingStoreDC(const QWidget *wizard, QPoint *offset);
#endif   // QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)

    bool hitTestContextHeaders(const QPoint &point) const;
    bool hitTestBackstageHeaders(const QPoint &point) const;

    bool isCompositionEnabled();
    bool isSmallSystemBorders();
    void enableWindowAero(HWND hwnd, bool enable);
    void enableWindowShadow(HWND hwnd, bool enable);

    bool setWindowAttribute(HWND hwnd, DWORD dwAttribute, LPCVOID pvAttribute, DWORD cbAttribute);
    bool resolveSymbols();
    void setFrameHook();

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    void saveMargins();
    void restoreMargins();
#endif   // QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)

protected Q_SLOTS:
    void refreshFrameTimer();
protected:
    virtual bool event(QEvent *event);
    virtual bool eventFilter(QObject *obj, QEvent *event);
public:
    static bool m_allowDwm;

    bool m_lockNCPaint;
    bool m_inUpdateFrame;
    bool m_inLayoutRequest;
    bool m_postReclalcLayout;
    bool m_active;
private:
    HICON m_hIcon;
    QSize m_szIcon;
    QWidget *m_frame;
    RibbonBar *m_ribbonBar;
    QList<ContextHeader *> *m_listContextHeaders;
    QSize m_szFrameRegion;
    QRect m_rcHeader;
    int m_frameBorder;
    int m_borderSizeBotton;
    HWND m_hwndFrame;
    QTimer m_refreshFrame;

    int m_oldMarginsleft;
    int m_oldMarginstop;
    int m_oldMarginsright;
    int m_oldMarginsbottom;

    bool m_dwmEnabled;
    bool m_skipNCPaint;
    bool m_officeFrameEnabled;
    bool m_isTransparentCaption;
    int m_shellAutohideBars;
    bool m_shellAutohideBarsInitialized;

    bool m_wasFullScreen;
    bool m_closeWindow;
    int m_changedSize;
private:
    class FrameHelperEventHook;
    Q_DISABLE_COPY(OfficeFrameHelperWin)
};

QRIBBON_END_NAMESPACE
