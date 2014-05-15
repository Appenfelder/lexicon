#ifndef __NOTEBROWSER_H__
#define __NOTEBROWSER_H__
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTableView>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QHeaderView>
#include <QFile>
#include <QMessageBox>
#include <QSqlError>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QSplitter>
#include <QTableWidget>
#include <QDebug>
#include <QModelIndex>
#include "QsLog.h"
#include "notes.h"
class NoteBrowser : public QWidget {
  Q_OBJECT
 public:

  NoteBrowser(QWidget * parent = 0);
  //  ~NotesWidget();
 public slots:
   void onNoteSelected(int );
   void onCellClicked(int,int);
   void onDeleteClicked();
   //  void onNewClicked();
  //  void onLexiconClicked();
  //  void showSelectedNote();
  //  void setActiveNode(const QString & node,const QString & word);
 private:
   void loadTable();
  //  int m_currentRow;
  //  QString m_baseQuery;
  //  QSqlDatabase m_db;
  //QSqlQueryModel * m_model;
  //  EditableSqlModel * m_model;
  QTableWidget * m_list;
  QTextEdit * m_note;
  QLineEdit * m_subject;
  QLineEdit * m_tag;
  QPushButton * m_newBtn;
  QPushButton * m_showBtn;
  QPushButton * m_saveBtn;
  QPushButton * m_deleteButton;
  QPushButton * m_printButton;
  QPushButton * m_viewButton;
  //  QPushButton * m_lexiconBtn;
  //  QWidget * createQueryWidget();
  //  QWidget * createEditWidget();
};

#endif