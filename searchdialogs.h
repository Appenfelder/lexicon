#ifndef __SEARCHDIALOG_H__
#define __SEARCHDIALOG_H__
#include <QWidget>
#include <QPushButton>
#include <QGroupBox>
#include <QRadioButton>
#include <QDialog>
#include <QCheckBox>
#include <QGridLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include "application.h"
#include "imedit.h"
#include "imlineedit.h"
#include "keyboardwidget.h"
#include "searchoptionswidget.h"
/*
class ArabicSearchDialog : public QDialog {
  Q_OBJECT

 public:
  ArabicSearchDialog(SearchOptions::SearchScope_t type,QWidget * parent = 0, Qt::WindowFlags f = 0);
  ~ArabicSearchDialog();
  QString getText() const;
  void setPrompt(const QString &);

  void getOptions(SearchOptions &) const;
  void setOptions(SearchOptions &);
  bool getForceLTR() const ;
  void setText(const QString &);
  void hideKeyboard();
  public slots:
    virtual void showOptions(bool);
    void showKeyboard();
    void loadKeymap(const QString &);
    void keyboardClosed();
    void onKeyboardShortcut(const QString &);
    void onHelp();
 protected:
    SearchOptions::SearchScope_t m_searchType;
    int m_count;
    KeyboardWidget * m_keyboard;
    bool m_attached;
    bool m_mapEnabled;
    QLabel * m_prompt;
    QGroupBox * m_group;
    ImLineEdit * m_edit;
    QDialogButtonBox * m_buttonBox;
    QPushButton * m_moreButton;
    QPushButton * m_findButton;
    QPushButton * m_keyboardButton;
    SearchOptionsWidget  * m_options;
 signals:
    void showHelp(const  QString &);
};
*/
class NodeSearchDialog : public QDialog {
  Q_OBJECT

 public:
  NodeSearchDialog(QWidget * parent = 0, Qt::WindowFlags f = 0);
  QString getText() const;
  void setText(const QString &);
  void getOptions(SearchOptions &);
  void setOptions();
  public slots:
  void checkOptions(int x = 0);
  public slots:
    void onHelp();
 private:
  void readSettings();
    int m_searchType;
  QLabel * m_prompt;
  QLineEdit * m_edit;
  QCheckBox * m_newTab;
  QCheckBox * m_switchFocus;
  QDialogButtonBox * m_buttonBox;
  QPushButton * m_findButton;
  QWidget * m_options;
 signals:
    void showHelp(const  QString &);
};
class PageSearchDialog : public QDialog {
  Q_OBJECT

 public:
  PageSearchDialog(QWidget * parent = 0, Qt::WindowFlags f = 0);
  int getPage() const;
  void getOptions(SearchOptions &);
  void setOptions();
  void setText(const QString &);
  public slots:
  void checkOptions(int x = 0);
  public slots:
    void onHelp();
 private:
  void readSettings();
  int m_searchType;
  QLabel * m_prompt;
  QLineEdit * m_edit;
  QCheckBox * m_newTab;
  QCheckBox * m_switchFocus;
  QDialogButtonBox * m_buttonBox;
  QPushButton * m_findButton;
  QWidget * m_options;
 signals:
    void showHelp(const  QString &);
};
#endif
