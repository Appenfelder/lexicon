#include "searchoptionswidget.h"
#include "fullsearch.h"
#include "scripts.h"
#include "QsLog.h"
#include "definedsettings.h"
#include "externs.h"
#include "application.h"
#define USE_KEYMAPS 0
/**
 * There are some unused options here that have been left in just in case.
 * In particular the plan to allow for searches of the Buckwalter text
 *
 *
 */
SearchOptionsWidget::SearchOptionsWidget(SearchOptions::SearchScope_t searchFor,QWidget * parent) : QWidget(parent) {
  QLOG_DEBUG()  << Q_FUNC_INFO << searchFor;
  setObjectName("searchoptionswidget");
  m_options.setSearchScope(searchFor);
  m_more = false;
  m_hasMaps = false;
  m_keymapsEnabled = false;
  setup(parent);
}
SearchOptionsWidget::SearchOptionsWidget(SearchOptions & options,QWidget * parent) : QWidget(parent) {
  QLOG_DEBUG() << Q_FUNC_INFO << options.getSearchScope();
  setObjectName("searchoptionswidget");
  m_more = false;
  m_options = options;
  m_hasMaps = false;
  m_keymapsEnabled = false;
  setup(parent);
}
void SearchOptionsWidget::setup(QWidget * parent) {
  QVBoxLayout * mainlayout = new QVBoxLayout;
  QGridLayout * gridlayout = new QGridLayout;

  m_targetGroup = new QGroupBox(tr("Search target"));
  m_targetGroup->setObjectName("searchtargetgroup");
  m_typeGroup = new QGroupBox(tr("Search type"));
  m_typeGroup->setObjectName("searchtypegroup");
  if (USE_KEYMAPS) {
    m_keymapGroup = new QGroupBox(tr("Keymap"));
    m_keymapGroup->setObjectName("keymapgroup");
  }
  m_ignoreDiacritics = new QCheckBox(tr("Ignore diacritics"));
  m_wholeWordMatch = new QCheckBox(tr("Whole word match"));
  m_headWord = new QRadioButton(tr("Head word"));
  m_fullText = new QRadioButton(tr("Full text"));
  m_normalSearch = new QRadioButton(tr("Normal"),m_typeGroup);
  m_regexSearch = new QRadioButton(tr("Regular expression"),m_typeGroup);
  m_ignoreCase = new QCheckBox(tr("Ignore case"));
  /*
    m_diacriticsText = new QLabel(tr("For Arabic text"));
    QFont f = m_diacriticsText->font();
    f.setItalic(true);
    m_diacriticsText->setFont(f);
    m_wholeText = new QLabel(tr("(For single word searches)"));
    m_wholeText->setFont(f);
    m_caseText = new QLabel(tr("(For English text)"));
    m_caseText->setFont(f);
  */
    m_headPhrase = new QCheckBox(tr("Search entry heading"));
  QHBoxLayout * typelayout = new QHBoxLayout;
  typelayout->addWidget(m_normalSearch);
  typelayout->addWidget(m_regexSearch);
  m_typeGroup->setLayout(typelayout);

  m_arabicTarget = new QRadioButton(tr("Arabic text"),m_targetGroup);
  m_buckwalterTarget = new QRadioButton(tr("Buckwalter transliteration"),m_targetGroup);

  m_includeHeads = new QCheckBox(tr("Include head entries in results"));
  m_showAllSearch = new QCheckBox(tr("Highlight all"));
  m_newTab = new QCheckBox(tr("Show result in new tab"));
  m_makeActive = new QCheckBox(tr("Got to new tab"));
  connect(m_newTab,SIGNAL(stateChanged(int)),this,SLOT(onNewTab(int)));

  m_forceLTR = new QCheckBox(tr("Force Left to Right on input"));
  connect(m_forceLTR,SIGNAL(stateChanged(int)),this,SLOT(onForceLeftToRight(int)));

  m_spacer = new QSpacerItem(0, 20,QSizePolicy::Ignored, QSizePolicy::MinimumExpanding);
  int row = 0;
  gridlayout->addWidget(m_typeGroup,row,0,2,2);
  row += 2;
  if ( ! m_options.textSearch()) {
    gridlayout->addWidget(m_ignoreDiacritics,row,0);
    gridlayout->addWidget(m_wholeWordMatch,row,1);
    row++;
    gridlayout->addWidget(m_forceLTR,row,0);
    row++;
    gridlayout->addWidget(m_includeHeads,row,0);
    row++;
    gridlayout->addWidget(m_headPhrase,row,0);
    row++;
    gridlayout->addWidget(m_showAllSearch,row,0);
    row++;
    gridlayout->addWidget(m_ignoreCase,row,0);
  }
  else {
    gridlayout->addWidget(m_ignoreDiacritics,row,0);
    //    gridlayout->addWidget(m_diacriticsText,row,1);
    row++;
    gridlayout->addWidget(m_ignoreCase,row,0);
    //    gridlayout->addWidget(m_caseText,row,1);
    row++;
    gridlayout->addWidget(m_wholeWordMatch,row,0);
    //    gridlayout->addWidget(m_wholeText,row,1);
  }
  row++;
  gridlayout->addWidget(m_newTab,row,0);
  gridlayout->addWidget(m_makeActive,row,1);
  row++;
  gridlayout->addWidget(m_arabicTarget,row,0);
  gridlayout->addWidget(m_buckwalterTarget,row,1);

  mainlayout->addLayout(gridlayout);
  mainlayout->addSpacerItem(m_spacer);

  setLayout(mainlayout);

  connect(m_normalSearch,SIGNAL(clicked()),this,SLOT(searchTypeChanged()));
  connect(m_regexSearch,SIGNAL(clicked()),this,SLOT(searchTypeChanged()));
  showMore(false);
}
SearchOptionsWidget::~SearchOptionsWidget() {
  QLOG_DEBUG() << Q_FUNC_INFO;
}
/**
 * This was originally written to have a 'More' button to show/hide
 * additional options. All options are now shown
 *
 *
 */
void SearchOptionsWidget::showMore(bool /* show */) {
  int type = m_options.getSearchScope();
  m_arabicTarget->hide();
  m_buckwalterTarget->hide();
  m_showAllSearch->hide();
  m_ignoreCase->hide();
  switch(type) {
  case SearchOptions::Root : {
    m_ignoreDiacritics->hide();
    m_wholeWordMatch->hide();
    m_typeGroup->hide();
    m_includeHeads->hide();
    m_forceLTR->hide();
    m_normalSearch->hide();
    m_regexSearch->hide();
    m_typeGroup->hide();
    m_headPhrase->hide();
    if (USE_KEYMAPS) {
      if (m_hasMaps)
        m_keymapGroup->setVisible(false);
    }
    break;
  }
  case SearchOptions::Entry : {
    m_includeHeads->setVisible(false);
    m_headPhrase->show();
    m_normalSearch->show();
    m_regexSearch->show();
    m_typeGroup->show();
      if (USE_KEYMAPS) {
      if (m_hasMaps && m_keymapsEnabled) {
        m_keymapGroup->setVisible(false);
      }
      else {
        m_keymapGroup->setVisible(false);
      }
    }
    if (m_regexSearch->isChecked()) {
      m_forceLTR->show();
      m_ignoreDiacritics->hide();
      m_wholeWordMatch->hide();
    }
    else {
      m_forceLTR->hide();
      m_ignoreDiacritics->show();
      m_wholeWordMatch->show();
    }
    break;
  }
  case SearchOptions::Word : {
    m_typeGroup->show();
    m_headPhrase->hide();

    if (USE_KEYMAPS) {
      if (m_hasMaps && m_keymapsEnabled)
        m_keymapGroup->setVisible(false);
      else
        m_keymapGroup->setVisible(false);
    }
    m_includeHeads->show();
    if (m_regexSearch->isChecked()) {
      m_forceLTR->show();
      m_ignoreDiacritics->hide();
      m_wholeWordMatch->hide();
    }
    else {
      m_forceLTR->hide();
      m_ignoreDiacritics->show();
      m_wholeWordMatch->show();
    }
    break;
  }
  case SearchOptions::Local : {
    m_headPhrase->hide();
    m_newTab->hide();
    m_makeActive->hide();
    m_showAllSearch->show();
    m_typeGroup->show();
    m_ignoreCase->show();
    if (USE_KEYMAPS) {
      if (m_hasMaps && m_keymapsEnabled)
        m_keymapGroup->setVisible(false);
      else
        m_keymapGroup->setVisible(false);
    }
    m_includeHeads->setVisible(false);

    if (m_regexSearch->isChecked()) {
      m_forceLTR->show();
      m_ignoreDiacritics->hide();
      m_wholeWordMatch->hide();
    }
    else {
      m_forceLTR->hide();
      m_ignoreDiacritics->show();
      m_wholeWordMatch->show();
    }
    break;
  }
  case SearchOptions::Text : {
    m_includeHeads->hide();
    m_ignoreDiacritics->show();
    m_ignoreCase->show();
    m_wholeWordMatch->show();
    m_headPhrase->hide();
    m_forceLTR->hide();
    m_ignoreDiacritics->setEnabled(! m_regexSearch->isChecked());
    m_ignoreCase->setEnabled(true);//! m_regexSearch->isChecked());
    m_wholeWordMatch->setEnabled(! m_regexSearch->isChecked());
    //    m_caseText->setEnabled(true); //! m_regexSearch->isChecked());
    //    m_wholeText->setEnabled(! m_regexSearch->isChecked());
    //    m_diacriticsText->setEnabled(! m_regexSearch->isChecked());
    break;
  }
  default :
    QLOG_WARN() << Q_FUNC_INFO << tr("Unknown search type") << type;
    break;
  }
}
void SearchOptionsWidget::searchTypeChanged() {
  showMore(m_more);
}
void SearchOptionsWidget::addKeymaps(const QString & activeMap,const QStringList & maps) {
  if (!USE_KEYMAPS)
    return;
  if (maps.size() > 0)
    m_hasMaps = true;

  QVBoxLayout * btnlayout = new QVBoxLayout;
  for(int i=0;i < maps.size();i++) {
    QRadioButton * btn = new QRadioButton(maps[i]);
    if (maps[i].toCaseFolded() == activeMap.toCaseFolded()) {
      btn->setChecked(true);
    }
    btnlayout->addWidget(btn);
    connect(btn,SIGNAL(clicked()),this,SLOT(keymapChanged()));
  }
  m_keymapGroup->setLayout(btnlayout);

  QVBoxLayout * layout = qobject_cast<QVBoxLayout *>(this->layout());
  if (layout) {
    layout->removeItem(m_spacer);
    layout->addWidget(m_keymapGroup);
    layout->addSpacerItem(m_spacer);
  }
}
void SearchOptionsWidget::keymapChanged() {
  if (!USE_KEYMAPS)
    return;
  QRadioButton * btn = qobject_cast<QRadioButton *>(QObject::sender());
  if (btn) {
    QLOG_DEBUG() << "keymap" << btn->text();
    emit(loadKeymap(btn->text()));
  }
}
void SearchOptionsWidget::setKeymapsEnabled(bool v) {
  m_keymapsEnabled = v;
}
void SearchOptionsWidget::onForceLeftToRight(int checked) {
  if (checked == Qt::Checked)
    emit(force(true));
  else
    emit(force(false));
}
bool SearchOptionsWidget::getForceLTR() {
  return m_forceLTR->isChecked();
}
void SearchOptionsWidget::onNewTab(int checked) {
  if (checked == Qt::Checked) {
    m_makeActive->setEnabled(true);
  }
  else {
    m_makeActive->setEnabled(false);
  }

}
void SearchOptionsWidget::getOptions(SearchOptions & opts) const {
  QLOG_DEBUG() << Q_FUNC_INFO;

  opts.setIgnoreDiacritics(true);
  opts.setSearchType(SearchOptions::Normal);
  opts.setIgnoreDiacritics(m_ignoreDiacritics->isChecked());
  opts.setWholeWordMatch(m_wholeWordMatch->isChecked());
  if (m_regexSearch->isChecked()) {
    opts.setSearchType(SearchOptions::Regex);
  }

  if (m_normalSearch->isChecked())
    opts.setSearchType(SearchOptions::Normal);

  // if (m_arabicTarget->isChecked())
  //   x |= Lane::Arabic;

  // if (m_buckwalterTarget->isChecked())
  //   x |= Lane::Buckwalter;

  opts.setIncludeHeads(m_includeHeads->isChecked());
  opts.setShowAll(m_showAllSearch->isChecked());

  opts.setKeymaps(m_keymapsEnabled);
  opts.setSearchScope(m_options.getSearchScope());

  opts.setNewTab(m_newTab->isChecked());
  opts.setActivateTab(m_makeActive->isChecked());
  opts.setShowAll(m_showAllSearch->isChecked());
  opts.setIgnoreCase(m_ignoreCase->isChecked());
  opts.setSearchScope(m_options.getSearchScope());
  opts.setHeadPhrase(m_headPhrase->isChecked());
}
void SearchOptionsWidget::setOptions(const SearchOptions & options) {
  QLOG_DEBUG() << Q_FUNC_INFO;
  m_options = options;
  m_ignoreDiacritics->setChecked(options.ignoreDiacritics());

  m_wholeWordMatch->setChecked(options.wholeWordMatch());

  m_regexSearch->setChecked((options.getSearchType() == SearchOptions::Regex));
  m_normalSearch->setChecked((options.getSearchType() == SearchOptions::Normal));


  m_newTab->setChecked(options.newTab());
  m_makeActive->setChecked(options.activateTab());
  m_makeActive->setEnabled(options.newTab());
  // m_arabicTarget->setChecked(x & Lane::Arabic);

  // m_buckwalterTarget->setChecked(x & Lane::Buckwalter);

  m_includeHeads->setChecked(options.includeHeads());


  m_keymapsEnabled = options.keymaps();

  m_showAllSearch->setChecked(options.showAll());

  m_ignoreCase->setChecked(options.ignoreCase());
  m_headPhrase->setChecked(options.headPhrase());
}
void SearchOptionsWidget::setOptions(int type) {
  QLOG_DEBUG() << Q_FUNC_INFO << type;

  SETTINGS

  settings.setIniCodec("UTF-8");
  switch (type) {
  case SearchOptions::Word : {
    /*
    settings.beginGroup("FullSearch");
    m_ignoreDiacritics->setChecked(settings.value(SID_FULLSEARCH_DIACRITICS,true).toBool());
    m_wholeWordMatch->setChecked(settings.value(SID_FULLSEARCH_WHOLE_WORD,true).toBool());
    m_regexSearch->setChecked(settings.value(SID_FULLSEARCH_TYPE_REGEX,false).toBool());
    m_normalSearch->setChecked(! m_regexSearch->isChecked());
    m_newTab->setChecked(settings.value(SID_FULLSEARCH_NEW_TAB,false).toBool());
    m_makeActive->setChecked(settings.value(SID_FULLSEARCH_GO_TAB,true).toBool());
    m_includeHeads->setChecked(settings.value(SID_FULLSEARCH_INCLUDE_HEADS,false).toBool());
    settings.endGroup();
    */
    break;
  }
  case SearchOptions::Entry : {
    settings.beginGroup("HeadSearch");
    m_ignoreDiacritics->setChecked(settings.value(SID_HEADSEARCH_DIACRITICS,true).toBool());
    m_wholeWordMatch->setChecked(settings.value(SID_HEADSEARCH_WHOLE_WORD,true).toBool());
    m_regexSearch->setChecked(settings.value(SID_HEADSEARCH_TYPE_REGEX,false).toBool());
    m_normalSearch->setChecked(! m_regexSearch->isChecked());
    m_newTab->setChecked(settings.value(SID_HEADSEARCH_NEW_TAB,false).toBool());
    m_makeActive->setChecked(settings.value(SID_HEADSEARCH_GO_TAB,true).toBool());
    m_headPhrase->setChecked(settings.value(SID_HEADSEARCH_USE_PHRASE,false).toBool());
    settings.endGroup();
    break;
  }
  case SearchOptions::Root : {
    settings.beginGroup("Search");
    m_newTab->setChecked(settings.value(SID_ROOTSEARCH_NEW_TAB,false).toBool());
    m_makeActive->setChecked(settings.value(SID_ROOTSEARCH_GO_TAB,true).toBool());
    settings.endGroup();
    break;
  }
  case SearchOptions::Local : {
    settings.beginGroup("LocalSearch");
    m_ignoreDiacritics->setChecked(settings.value(SID_LOCALSEARCH_DIACRITICS,true).toBool());
    m_wholeWordMatch->setChecked(settings.value(SID_LOCALSEARCH_WHOLE_WORD,true).toBool());
    m_regexSearch->setChecked(settings.value(SID_LOCALSEARCH_TYPE_REGEX,false).toBool());
    m_normalSearch->setChecked(! m_regexSearch->isChecked());
    m_showAllSearch->setChecked(settings.value(SID_LOCALSEARCH_SHOW_ALL,true).toBool());
    m_ignoreCase->setChecked(settings.value(SID_LOCALSEARCH_IGNORE_CASE,true).toBool());
    settings.endGroup();
    break;
  }
  case SearchOptions::Text : {
    settings.beginGroup("TextSearch");
    m_ignoreDiacritics->setChecked(settings.value(SID_TEXTSEARCH_DIACRITICS,true).toBool());
    m_wholeWordMatch->setChecked(settings.value(SID_TEXTSEARCH_WHOLE_WORD,true).toBool());
    m_regexSearch->setChecked(settings.value(SID_TEXTSEARCH_TYPE_REGEX,false).toBool());
    m_normalSearch->setChecked(! m_regexSearch->isChecked());
    m_ignoreCase->setChecked(settings.value(SID_TEXTSEARCH_IGNORE_CASE,true).toBool());
    settings.endGroup();
    break;
  }
  default : break;
  }
}
void SearchOptionsWidget::hideTabOptions(bool visible) {
  m_newTab->setVisible(visible);
  m_makeActive->setVisible(false);
}
bool SearchOptionsWidget::isTextSearch() const {
  return m_textSearch;
}
void SearchOptionsWidget::setTextSearch(bool v)  {
  m_textSearch = v;
}
