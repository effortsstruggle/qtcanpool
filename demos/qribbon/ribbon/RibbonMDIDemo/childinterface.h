#ifndef CHILDINTERFACE_H
#define CHILDINTERFACE_H

#include "qribbon/ribbon.h"

class QStatusBar;
namespace QRibbon
{
class RibbonBar;
class RibbonGroup;
class RibbonPage;
}

/* ChildInterface */
class ChildInterface
{
public:
    virtual ~ChildInterface()
    {
    }
    enum TypeDocument {
        td_None,
        td_Text,
        td_Sketch,
    };
public:
    virtual TypeDocument typeDocument() const = 0;
    virtual void buildUserInterface(QRibbon::RibbonBar *pRib, QStatusBar *pStatusBar,
                                    QRibbon::RibbonPage *pageHome) = 0;
    virtual void removeUserInterface(QRibbon::RibbonBar *pRib, QStatusBar *pStatusBar,
                                     QRibbon::RibbonPage *pageHome) = 0;

    virtual bool save() = 0;
    virtual bool saveAs() = 0;

    virtual QString userFriendlyCurrentFile() = 0;
    virtual QString currentFile() const = 0;

    virtual bool hasSelection() const = 0;
};
Q_DECLARE_INTERFACE(ChildInterface, "com.ribbon.ribbonmdisample.ChildInterface")

#endif   // INTERFACECHILD_H
