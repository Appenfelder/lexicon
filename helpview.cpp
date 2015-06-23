#include "helpview.h"
#include "application.h"
#include "externs.h"
#include "definedsettings.h"
#include "QsLog.h"
HelpView::HelpView(QWidget * parent) : QWidget(parent) {
  QVBoxLayout * layout = new QVBoxLayout;

  m_forwardButton = new QPushButton(">");
  m_backButton = new QPushButton("<");

  connect(m_forwardButton,SIGNAL(clicked()),this,SLOT(onPageForward()));
  connect(m_backButton,SIGNAL(clicked()),this,SLOT(onPageBack()));
  QHBoxLayout * btnlayout = new QHBoxLayout;
  btnlayout->addWidget(m_backButton);
  btnlayout->addWidget(m_forwardButton);
  btnlayout->addStretch();
  layout->addLayout(btnlayout);


  m_view = new QWebView(this);

  QDialogButtonBox * btns = new QDialogButtonBox(QDialogButtonBox::Close);
  layout->addWidget(m_view);
  layout->addWidget(btns);
  setLayout(layout);
  m_initialPage = true;
  m_timer = 0;
  m_progress = 0;

  connect(m_view,SIGNAL(linkClicked(const QUrl &)),this,SLOT(linkclick(const QUrl &)));
  connect(btns, SIGNAL(rejected()), this, SLOT(onClose()));
  connect(m_view,SIGNAL(loadProgress(int)),this,SLOT(loadProgress(int)));
  connect(m_view,SIGNAL(loadStarted()),this,SLOT(loadStarted()));
  connect(m_view,SIGNAL(loadFinished(bool)),this,SLOT(loadFinished(bool)));
  if (m_initialPage) {
    this->hide();
    m_view->hide();
  }

  readSettings();
  /**
   * check the current page points to an actual file. e.g. when the saved page url is out
   * of date after documentation changes
   *
   * If there is a problem loading the page, it seems to default to about:blank, so
   * check for this as well.
   */

  if (m_localSource && ! m_currentLocalPage.isEmpty()) {
    QFileInfo cp(m_currentLocalPage.toLocalFile());
    if (! cp.exists()) {
      m_currentLocalPage.clear();
    }
  }
  //  m_localPrefix = "file:///";
  //  m_localPrefix = "http://127.0.0.1:8000/";
  QString prefix;
  QString location;
  QUrl startPage;
  if (m_localSource) {
    prefix = m_localPrefix;
    location = m_localRoot;
    startPage = m_currentLocalPage;
  }
  else {
    prefix = m_onlinePrefix;
    location = m_onlineRoot;
    startPage = m_currentOnlinePage;
  }
  /*
  qDebug() << "Offline" << m_localSource;
  qDebug() << "Prefix" << prefix;
  qDebug() << "Location" << location;
  */
  if (startPage.isEmpty() || (startPage == QUrl("about:blank"))) {
    if (m_localSource) {
      if (location.endsWith(QDir::separator())) {
        location.chop(1);
      }
      QFileInfo fi(m_localRoot + QDir::separator() + "index.html");
      QLOG_DEBUG() << Q_FUNC_INFO << "Loading" << fi.absoluteFilePath();
      startPage = QUrl(m_localPrefix + fi.absoluteFilePath());
    }
    else {
      startPage = QUrl(prefix + "/" + "index.html");
    }
  }
  qDebug() << "Start page" << startPage;
  m_view->load(startPage);
  m_view->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
  m_forwardButton->setEnabled(false);
  m_backButton->setEnabled(false);
}
HelpView::~HelpView() {
  qDebug() << Q_FUNC_INFO;
  writeSettings();
}
void HelpView::linkclick(const QUrl & url) {
  qDebug() << Q_FUNC_INFO << url;

  QString str = url.toString();
  if (str.endsWith(QDir::separator())) {
    str.chop(1);
  }
  QUrl f(str + QDir::separator() + "index.html");
  m_view->load(f);
}
void HelpView::onClose() {
  this->hide();
}
void HelpView::readSettings() {
  SETTINGS

  settings.beginGroup("Help");
  m_localSource = settings.value(SID_HELP_LOCAL,true).toBool();


  m_localRoot = settings.value(SID_HELP_LOCAL_LOCATION,"site").toString();
  m_localPrefix = settings.value(SID_HELP_LOCAL_PREFIX,"file://").toString();
  m_currentLocalPage = settings.value(SID_HELP_LOCAL_URL,QUrl()).toUrl();

  m_onlineRoot = settings.value(SID_HELP_ONLINE_LOCATION,QString()).toString();
  m_onlinePrefix = settings.value(SID_HELP_ONLINE_PREFIX,"http://").toString();
  m_currentOnlinePage = settings.value(SID_HELP_ONLINE_URL,QUrl()).toUrl();
  resize(settings.value(SID_HELP_SIZE,QSize(800,600)).toSize());
  move(settings.value(SID_HELP_POS,QSize(450,20)).toPoint());
}
void HelpView::writeSettings() {
  SETTINGS

  settings.beginGroup("Help");
  settings.setValue(SID_HELP_SIZE, size());
  settings.setValue(SID_HELP_POS, pos());
  if (m_localSource) {
    settings.setValue(SID_HELP_LOCAL_URL, m_view->url());
  }
  else {
    settings.setValue(SID_HELP_ONLINE_URL, m_view->url());
  }
  settings.endGroup();

}
/**
 * The first load can sometimes take a few seconds so we are showing
 * a progress dialog
 *
 * @param x
 */
void HelpView::loadProgress(int x) {
  if (m_progress) {
    m_progress->setValue(x);
  }
}
void HelpView::loadStarted() {
  if (m_initialPage) {
    m_progress = new QProgressDialog("Loading ...", "Cancel", 0, 100);
    //    m_progress->setCancelButton(0);
    m_timer = new QTimer(this);
    connect(m_progress, SIGNAL(canceled()), this, SLOT(onCancel()));
    m_timer->start(0);
    m_progress->show();
  }
}
void HelpView::loadFinished(bool ok) {
  if (m_initialPage) {
    m_view->show();
    m_initialPage = false;
    m_timer->stop();
    delete m_progress;
    delete m_timer;
    m_timer = 0;
    m_progress = 0;
  }
  this->show();
  m_view->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
  /*
  QList<QWebHistoryItem> h = m_view->page()->history()->items();
  for(int i=0;i < h.size();i++) {
    qDebug() << h[i].url();
  }
  */
  m_forwardButton->setEnabled(m_view->page()->history()->canGoForward());
  m_backButton->setEnabled(m_view->page()->history()->canGoBack());
  emit(finished(ok));
}
bool HelpView::isLoaded() const {
  return ! m_initialPage;
}
bool HelpView::isOffline() const {
  return m_localSource;
}
void HelpView::onCancel() {
    m_initialPage = true;
    m_timer->stop();
    delete m_progress;
    delete m_timer;
    m_timer = 0;
    m_progress = 0;
    this->hide();
    /// TODO need to either delete the window
    /// or some way or other force a reload when
    emit(finished(false));
}
void HelpView::showEvent(QShowEvent * event) {
  qDebug() << Q_FUNC_INFO << m_initialPage;
  QWidget::showEvent(event);
}
void HelpView::onPageForward() {
  if (m_view->page()->history()->canGoForward()) {
    m_view->page()->history()->forward();
  }
}
void HelpView::onPageBack() {
  if (m_view->page()->history()->canGoBack()) {
    m_view->page()->history()->back();
  }
}
