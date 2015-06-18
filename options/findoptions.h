#ifndef __FINDOPTIONS_H__
#define __FINDOPTIONS_H__
#include "optionswidget.h"
#include <QFontDialog>
#include <QPushButton>
class FindOptions : public OptionsWidget {
  Q_OBJECT

 public:
  FindOptions(const QString & theme,QWidget * parent = 0);
  public slots:
    void writeSettings(const QString & fileName = QString());
    void readSettings();
    bool isModified();
    void onSetFont();
    void onSetColor();
    void onHeadDialog();
    void onFullDialog();
    void onLocalDialog();
 private:
    /// full search
    QCheckBox * m_fullDebug;
    QSpinBox  *  m_fullFragmentSize;
    QCheckBox * m_fullIncludeHeads;
    QCheckBox * m_fullOneRow;
    QSpinBox  * m_fullStep;
    //    QLineEdit * m_fullViewerWidth;
    //    QLineEdit * m_fullViewerHeight;
    //    QLineEdit * m_fullXslt;
    QLineEdit * m_fullHeadColor;
    QLineEdit * m_fullHeadText;
    QLineEdit * m_contextStyle;

    bool m_fullWholeWord;
    bool m_fullDiacritics;
    bool m_fullRegex;
    bool m_fullNewTab;
    bool m_fullGoTab;
    bool m_fullForce;

    /// head word search
    QCheckBox * m_headDebug;
    QSpinBox  * m_headStep;
    QCheckBox * m_headVertical;
    QCheckBox * m_headFocusTable;
    QCheckBox * m_headSingleClick;
    bool m_headWholeWord;
    bool m_headDiacritics;
    bool m_headRegex;
    bool m_headNewTab;
    bool m_headGoTab;
    bool m_headForce;

    /// local search
    bool m_localForce;
    bool m_localWholeWord;
    bool m_localRegex;
    bool m_localDiacritics;

    QCheckBox * m_localShowAll;


    QCheckBox * m_nodeNew;
    QCheckBox * m_nodeGo;
    QCheckBox * m_pageNew;
    QCheckBox * m_pageGo;
    QCheckBox * m_rootNew;
    QCheckBox * m_rootGo;

    //    QSize       m_viewerSize;
    //    QPoint      m_viewerPosition;

};
#endif
