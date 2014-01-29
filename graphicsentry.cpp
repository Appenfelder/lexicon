#include "graphicsentry.h"
EntryItem::EntryItem(const QString & text, QGraphicsItem * parent) : QGraphicsTextItem(text,parent) {
  m_supplement = 0;
}
EntryItem::EntryItem(QGraphicsItem * parent) :QGraphicsTextItem(parent) {
  m_supplement = 0;
}
void EntryItem::contextMenuEvent(QGraphicsSceneContextMenuEvent * event ) {
  //  QGraphicsTextItem::contextMenuEvent(event);

  QMenu menu;
  menu.setObjectName("entry");
  QAction *markAction = menu.addAction("Bookmark");
  QAction *searchAction = menu.addAction("Find");
  connect(searchAction,SIGNAL(triggered()),this,SLOT(searchItem()));

  if (this->textCursor().hasSelection()) {
    QAction *copyAction = menu.addAction("Copy");
    connect(copyAction,SIGNAL(triggered()),this,SLOT(copy()));
  }
  QAction *selectedAction = menu.exec(event->screenPos());
}
void EntryItem::searchItem() {
  qDebug() << Q_FUNC_INFO;
}
void EntryItem::hoverEnterEvent(QGraphicsSceneHoverEvent * event) {
  //  QLOG_DEBUG() << "hover" << this->getNode() << this->isRoot();
}
void EntryItem::setRoot(const QString & root,bool isRootEntry) {
  m_root = root;
  m_isRoot = isRootEntry;
}
void EntryItem::copy() {
  QString txt = this->textCursor().selectedText();
  QClipboard *clipboard = QApplication::clipboard();
  clipboard->setText(txt);
}
void EntryItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *o, QWidget *w) {
  QPen pen = painter->pen();
  QBrush brush = painter->brush();
  painter->setPen(Qt::NoPen);
  if ( ! m_supplement) {
    painter->setBrush(Qt::white);
  }
  else {
    painter->setBrush(m_backgroundColor);
  }
  painter->drawRect(boundingRect());
  painter->setPen(pen);
  QGraphicsTextItem::paint(painter, o, w);
}
QTextCursor EntryItem::highlight(const QString & target) {
  int pos;
  QTextCursor cursor;
  QTextCursor firstPos;
  QTextDocument * doc = this->document();
  QTextCharFormat fmt;
  /// TODO get from QSettings
  fmt.setBackground(Qt::yellow);
  cursor = doc->find(target,QTextDocument::FindWholeWords);
  firstPos = cursor;
  while(! cursor.isNull()) {
    pos =  cursor.position();
    //    qDebug() << Q_FUNC_INFO << "found at" << pos;
    cursor.setPosition(pos - target.size(), QTextCursor::MoveAnchor);
    cursor.setPosition(pos, QTextCursor::KeepAnchor);
    cursor.setCharFormat(fmt);
    cursor = doc->find(target,pos,QTextDocument::FindWholeWords);
  }
  return firstPos;
}
/**
 * this is the one that is used by default
 *
 * @param target
 *
 * @return
 */
QTextCursor EntryItem::highlightRx(const QString & target) {
  int pos;
  QTextCursor cursor;
  QTextCursor firstPos;
  QTextDocument * doc = this->document();
  QTextCharFormat fmt;
  QRegExp rx;
  /// TODO this needs to be fixed to allow for e.g. full stop
  /// and then made the default
  rx.setPattern("\\b" + target + "\\b");
  /// TODO get from QSettings
  fmt.setBackground(Qt::yellow);
  cursor = doc->find(rx,QTextDocument::FindWholeWords);
  firstPos = cursor;
  while(! cursor.isNull()) {
    pos =  cursor.position();
    cursor.setPosition(pos - target.size(), QTextCursor::MoveAnchor);
    cursor.setPosition(pos, QTextCursor::KeepAnchor);
    cursor.setCharFormat(fmt);
    cursor = doc->find(target,pos,QTextDocument::FindWholeWords);
  }
  return firstPos;
}
LaneGraphicsView::LaneGraphicsView(QGraphicsScene * scene,GraphicsEntry * parent) :
  QGraphicsView(scene,parent) {
  //setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}
void LaneGraphicsView::scrollContentsBy(int dx,int dy) {
  QGraphicsView::scrollContentsBy(dx,dy);
  /*
  qDebug() << Q_FUNC_INFO << dx << dy;
  QScrollBar * b = this->verticalScrollBar();
  if (b->value() == b->maximum()) {
    qDebug() << "nextPage emit";
    emit(nextPage());
  }
  else if (b->value() == b->minimum()) {
    qDebug() << "backPage emit";
    emit(backPage());
  }
  else {

  }
  */
}
void LaneGraphicsView::keyPressEvent(QKeyEvent * event) {
  //  QLOG_DEBUG() << "got key";
  QGraphicsView::keyPressEvent(event);
}
GraphicsEntry::GraphicsEntry(QWidget * parent ) : QWidget(parent) {
  readSettings();
  QVBoxLayout * layout = new QVBoxLayout;
  m_textOption.setTextDirection(Qt::LeftToRight);
  m_compiledXsl = 0;
  /// 0 = paging forward, items are appended
  /// 1 = paging backward, items are prepended
  m_pagingDir = 0;
  m_scale = 1.0;

  QHBoxLayout * btnslayout = new QHBoxLayout;
  m_zoomIn = new QPushButton(QIcon("./images/zoom-in-32.png"),tr(""));
  m_zoomIn->setToolTip(tr("Zoom in"));
  m_zoomOut = new QPushButton(QIcon("./images/zoom-out-32.png"),tr(""));
  m_zoomOut->setToolTip(tr("Zoom out"));
  m_clearSceneBtn = new QPushButton(QIcon("./images/32/cleaning-erase-eraser-icone-4970-32.png"),tr("Clear"));
  m_clearSceneBtn->setToolTip("Clear contents");
  btnslayout->addWidget(m_zoomIn);
  btnslayout->addWidget(m_zoomOut);
  btnslayout->addWidget(m_clearSceneBtn);
  layout->addLayout(btnslayout);

  connect(m_zoomIn,SIGNAL(clicked()),this,SLOT(onZoomIn()));
  connect(m_zoomOut,SIGNAL(clicked()),this,SLOT(onZoomOut()));
  connect(m_clearSceneBtn,SIGNAL(clicked()),this,SLOT(onClearScene()));


  m_scene = new QGraphicsScene(this);
  m_view = new LaneGraphicsView(m_scene,this);

  //  m_view->setBackgroundBrush(QBrush(Qt::red, Qt::SolidPattern));
  connect(m_view,SIGNAL(nextPage()),this,SLOT(nextPageRequested()));
  connect(m_view,SIGNAL(backPage()),this,SLOT(prevPageRequested()));

  m_view->setInteractive(true);
  m_item = new QGraphicsTextItem("");
  m_item->setTextInteractionFlags(Qt::TextBrowserInteraction);
  m_item->setTextWidth(m_textWidth);
  m_scene->addItem(m_item);

  // add the graphics viwe
  layout->addWidget(m_view,1);

  m_nodeQuery = 0;
  setLayout(layout);

  connect(m_item,SIGNAL(linkActivated(const QString &)),this,SLOT(linkActivated(const QString &)));
  connect(m_item,SIGNAL(linkHovered(const QString &)),this,SLOT(linkHovered(const QString &)));

  connect(m_scene,SIGNAL(focusItemChanged(QGraphicsItem *, QGraphicsItem *, Qt::FocusReason)),
          this,SIGNAL(focusItemChanged(QGraphicsItem *, QGraphicsItem *, Qt::FocusReason)));

  m_xalan = getXalan();
  prepareQueries();
}
GraphicsEntry::~GraphicsEntry() {
  qDebug() << Q_FUNC_INFO;
  delete m_nodeQuery;
  delete m_rootQuery;
  delete m_nextRootQuery;
  delete m_pageQuery;
  /// TODO xalan cleanup ?
}
void GraphicsEntry::readSettings() {
  QSettings settings;
  settings.beginGroup("Entry");
  QString css = settings.value("css",QString("entry.css")).toString();
  readCssFromFile(css);
  m_xsltSource = settings.value("xslt",QString("entry.xslt")).toString();
  m_textWidth = settings.value("text width",300).toInt();
  m_debug = settings.value("debug",false).toBool();
  m_entryMargin = settings.value("margin",10).toInt();

  QString bg = settings.value("Supplement Background Color",QString("rgb(255,254,253)")).toString();
  QRegExp rx("^rgb\\((\\d+),\\s*(\\d+),\\s*(\\d+)\\s*\\)");
  if (rx.indexIn(bg) != -1) {
    //    qDebug() << bg << "color" << rx.cap(1) << rx.cap(2) << rx.cap(3);
    m_supplementBg = QColor::fromRgb(rx.cap(1).toInt(),rx.cap(2).toInt(),rx.cap(3).toInt());
    if (! m_supplementBg.isValid())  {
      m_supplementBg = QColor::fromRgb(255,255,255);
    }
  }
  else {
    m_supplementBg = QColor::fromRgb(255,255,255);
  }
  m_clearScene = settings.value("clear",true).toBool();
}
void GraphicsEntry::writeDefaultSettings() {
  QSettings settings;
  settings.beginGroup("Entry");
  settings.setValue("css","entry.css");
  settings.setValue("xslt","entry.xslt");
  settings.setValue("debug",true);
  settings.setValue("text width",300);
  settings.setValue("clear",true);
}
void GraphicsEntry::keyPressEvent(QKeyEvent * event) {
  switch(event->key()) {
  case Qt::Key_Plus: {
    onZoomIn();
    break;
  }
  case Qt::Key_Minus: {
    onZoomOut();
    break;
  }
  default:
    QWidget::keyPressEvent(event);
  }

}
void GraphicsEntry::focusInEvent(QFocusEvent * event) {
  QLOG_DEBUG() << "Got input focus";
  m_view->setFocus();
  QWidget::focusInEvent(event);

}
void GraphicsEntry::onClearScene() {
  for(int i=0;i < m_items.size();i++) {
    m_scene->removeItem(m_items[i]);
  }
  while(m_items.size() > 0) {
    delete m_items.takeFirst();
  }
}
void GraphicsEntry::anchorClicked(const QUrl & link) {
  QLOG_DEBUG() << link.toDisplayString();
  QLOG_DEBUG() << QApplication::keyboardModifiers();
}
void GraphicsEntry::linkActivated(const QString & link) {
  /// turn history on as the user has clicked on something
  getHistory()->on();
  QString node(link);
  /// remove the leading #
  node.remove(0,1);
  Place p;
  p.setNode(node);
  showPlace(p);
}
void GraphicsEntry::linkHovered(const QString & link) {
  QGraphicsTextItem * gi = static_cast<QGraphicsTextItem *>(QObject::sender());

  if (link.isEmpty()) {
    gi->setCursor(QCursor(Qt::ArrowCursor));
  }
  else {
    gi->setCursor(QCursor(Qt::PointingHandCursor));
    QLOG_DEBUG() << "Link hovered" << link;
    QLOG_DEBUG() << QApplication::keyboardModifiers();
  }
}
/**
 * redundant
 *
 */
void GraphicsEntry::anchorTest() {
  QString node;
  //  QList<QGraphicsItem *> items = m_scene->items();
  for(int i=0;i < m_items.size();i++) {
    EntryItem * item = m_items[i];
    if (item) {
      if (item->isNode(node)) {
        m_scene->setFocusItem(item);
        m_view->ensureVisible(item);
        return;
      }
    }
  }
}
bool GraphicsEntry::showPlace(const Place & p,bool thisPageOnly) {
  /// check if the node is on this page
  QString node = p.getNode();
  for(int i=0;i < m_items.size();i++) {
    EntryItem * item = m_items[i];
    if (item->isNode(node)) {
      QLOG_DEBUG() << "Found local node" << node;
      m_scene->setFocusItem(item);
      //m_view->ensureVisible(item);
       m_view->centerOn(item);
      //     m_scene->clearFocus();
      return true;
    }
  }
  QLOG_DEBUG() << "Not found on page" << node;
  /// trying out of page jump
  /// TODO this does not return Place needs fixing
  if (! thisPageOnly ) {
    QLOG_DEBUG() << "Out of page jump" << node;
    getXmlForNode(node);
  }
  return false;
}
/**
 *
 *
 */
bool GraphicsEntry::readCssFromFile(const QString & name) {
  QFile f(name);
  if (! f.open(QIODevice::ReadOnly)) {
    QLOG_WARN()  << "Cannot open file for reading: "
                 << f.errorString();
    return false;

  }
  QTextStream in(&f);
  QString css;
  while( ! in.atEnd()) {
    if (! css.startsWith("-")) {
      css += in.readLine();
    }
  }
  f.close();
  if (! css.isEmpty()) {
    m_currentCSS= css;
    emit(cssChanged());
  }
  return true;
}



bool GraphicsEntry::prepareQueries() {
  bool ok;
  m_pageQuery = new QSqlQuery;
  ok = m_pageQuery->prepare("select root,broot,word,bword,xml,page,itype,nodeId,supplement from entry where page = ? order by nodenum asc");
  if (! ok ) {
    QLOG_WARN() << "page SQL prepare failed" << m_pageQuery->lastError();
  }
  m_nodeQuery = new QSqlQuery;
  ok = m_nodeQuery->prepare("select * from entry where nodeId = ?");
  if (! ok ) {
    QLOG_WARN() << "node SQL prepare failed" << m_nodeQuery->lastError();
  }
  m_rootQuery = new QSqlQuery;
  ok = m_rootQuery->prepare("select root,broot,word,bword,xml,page,itype,nodeId,supplement from entry where root = ? order by nodenum");
  if (! ok ) {
    QLOG_WARN() << "root SQL prepare failed" << m_rootQuery->lastError();
  }
  m_nextRootQuery = new QSqlQuery;
  ok = m_nextRootQuery->prepare("select word from root ");
  if (! ok ) {
    QLOG_WARN() << "next root SQL prepare failed" << m_nextRootQuery->lastError();
  }
  return ok;
}
Place GraphicsEntry::getXmlForPlace(const Place & p) {
  Place np = getXmlForRoot(p);
  if (np.isValid()) {
    if (np.getRoot() != m_place.getRoot()) {
      emit(placeChanged(np));
    }
    m_place = np;
  }
  return np;
}
/**
 * get the root,supplement for the given node and show it
 * nodeId are unique
 *
 * @param node
 *
 */
Place GraphicsEntry::getXmlForNode(const QString  & node,bool nodeOnly) {
  Place p;
  QLOG_DEBUG() << "Search for node" << node << nodeOnly;
  m_nodeQuery->bindValue(0,node);
  m_nodeQuery->exec();

  if (m_nodeQuery->first()) {
    //QString xml = m_nodeQuery->value("xml").toString();
    //    QLOG_DEBUG() << "got " << xml;
    QString root = m_nodeQuery->value("root").toString();
    int supplement = m_nodeQuery->value("supplement").toInt();
    p = Place(root,supplement);
    p.setNode(node);
    p.setWord(m_nodeQuery->value("word").toString());
    p.setNodeOnly(nodeOnly);
    Place np = getXmlForPlace(p);
    /// focus on the node, set thisPageOnly to true just in case something has gone horribly wrong
    /// otherwise we'll be looping forever
    showPlace(np,true);
  }
  else {
    QLOG_DEBUG() << "Error" << m_nodeQuery->lastError().text();
  }
  return p;
}
/**
 *
 *
 * @param root
 * @param node the id of the entry we want to focus on
 */
Place GraphicsEntry::getXmlForRoot(const Place & dp) {
  QList<EntryItem *> items;
  QString arRoot;
  int itemCount;
  int supplement;
  QString str;
  EntryItem * centerItem;
  Place p;

  QString root = dp.getRoot();
  int supp = dp.getSupplement();
  QString node = dp.getNode();
  bool nodeOnly = dp.getNodeOnly();

  QLOG_DEBUG() << Q_FUNC_INFO << root << supp << node << "nodeOnly = " << nodeOnly;
  m_rootQuery->bindValue(0,root);
  m_rootQuery->exec();
  if (! m_rootQuery->first()) {
    QLOG_INFO() << "root not found" << root;
    return p;
  }
  if (m_clearScene) {
    onClearScene();
  }
  /// this will be set to the right word if a node has been supplied
  QString showWord;
  QString startNode = node;
  /// get the position of the last item in the scene
  itemCount = m_items.size();

  /// add the root item unless nodeOnly is set
  str = QString("<word type=\"root\" ar=\"%1\" />").arg(root);
  EntryItem * rootItem  = createEntry(str);
  rootItem->setRoot(root,true);
  rootItem->setSupplement(supp);
  p.setRoot(root);
  p.setSupplement(supp);
  if (! nodeOnly ) {
    items << rootItem;
  }
  /// by default we will center on the root item
  centerItem = rootItem;
  /// now add all the entries for the root
  bool ok;
  do  {
    ok = true;
    supplement = m_rootQuery->value(8).toInt();
    /**
     * if we've asked to see the entry for the supplement then just show that
     *
     */
    if ((supp == 1) && (supplement == 0)) {
      ok = false;
    }
    if (ok) {
      arRoot = m_rootQuery->value(0).toString();
      QLOG_DEBUG() << m_rootQuery->value(3).toString();
      QString t  = QString("<word buck=\"%1\" ar=\"%2\" page=\"%3\" itype=\"%4\" supp=\"%5\">")
        .arg(m_rootQuery->value(3).toString())
        .arg(m_rootQuery->value(2).toString())
        .arg(m_rootQuery->value(5).toString())
        .arg(m_rootQuery->value(6).toString())
        .arg(m_rootQuery->value(8).toInt());
      t += m_rootQuery->value(4).toString();
      t += "</word>";
      if (m_debug) {
        QFileInfo fi(QDir::tempPath(),QString("/tmp/%1.xml").arg(m_rootQuery->value(7).toString()));
        QFile f(fi.filePath());
        if (f.open(QIODevice::WriteOnly)) {
          QTextStream out(&f);
          out.setCodec("UTF-8");
          out << t;
        }
      }
      EntryItem * item  = createEntry(t);
      item->setSupplement(supplement);
      item->setNode(m_rootQuery->value(7).toString());
      /// get the nodeid of the first item at added, so we jump to it later
      item->setRoot(arRoot);
      item->setWord(m_rootQuery->value(2).toString());
      if (startNode.isEmpty()) {
        startNode = m_rootQuery->value(7).toString();
      }
      else if ( startNode == item->getNode()) {
        showWord = item->getWord();
        /// if a node has been passed, then center on the node
        centerItem = item;
        p.setNode(startNode);
        if ( nodeOnly ) {
          items << item;
        }
      }
      if (! nodeOnly ) {
        items << item;
      }
    }
  } while(m_rootQuery->next());

  /// TODO we've only got a root item
  if (items.size() == 1) {
  }
  /**
   * we have all the new items to show.
   * If we are going forward, add them to the end
   *                 backwards, prepend them in reverse order
   *
   */

  if (m_pagingDir == 1) {
    /// where the old items begin
    int x = items.size();
    while(items.size() > 0) {
      EntryItem * item = items.takeLast();
      m_items.prepend(item);
    }
    prependEntries(x);
  }
  else {
    while(items.size() > 0) {
      EntryItem * item = items.takeFirst();
      m_items.append(item);
    }
    /// where the old when finish
    //
    appendEntries(itemCount);
  }

  m_view->setFocus();
  m_transform = m_view->transform();

  //QLOG_DEBUG() << "Scene rect" << m_scene->sceneRect();
  /// without thus centerOn() does not work properly for
  /// items added to the scene
  m_view->setSceneRect(m_scene->sceneRect());
  if (m_currentRoot != arRoot) {
    emit rootChanged(arRoot,node);
    m_currentRoot = arRoot;
  }
  /**
   * we need to know whether we got here by accessing the history button
   * or not
   *
   */
  if (getHistory()->isOn()) {
    HistoryEvent * event = new HistoryEvent;
    event->setRoot(arRoot);
    event->setNode(node);
    event->setWord(showWord);
    getHistory()->add(event);
  }
  /// TODO there has to be a better way than this
  /// TODO if node specified we need to center on it
  if ( node.isEmpty()) {
    /*
    QTextCursor cursor(centerItem->document());
    QTextBlock b = cursor.block();
    QTextLayout * layout = b.layout();
    QTextLine line = layout->lineForTextPosition(cursor.position());
    if (line.isValid()) {
      qDebug() << Q_FUNC_INFO << "text line pos" << line.position() << line.textLength();
        m_view->centerOn(line.position());
    }
    */

      m_scene->setFocusItem(centerItem);
      int h =  m_view->height();
      QPointF p = centerItem->pos();
      p.setY(p.y() + h/2 - 10);
      m_view->centerOn(p);
  }
  p.setPage(centerItem->getPage());
  if (nodeOnly) {
    delete rootItem;
  }
  return p;
}
/**
 * create the QTextGraphicsItem by transforming the passed xml
 *
 * @param xml
 *
 * @return
 */
EntryItem * GraphicsEntry::createEntry(const QString & xml) {
    QString html =transform(m_xsltSource,xml);
    EntryItem * gi = new EntryItem("");
    gi->document()->setDefaultStyleSheet(m_currentCSS);
    gi->setTextWidth(m_textWidth);
    gi->setHtml(html);
    /// need this otherwise arabic text will be right justified
    gi->document()->setDefaultTextOption(m_textOption);
    gi->setTextInteractionFlags(Qt::TextBrowserInteraction);
    gi->setAcceptHoverEvents(true);
    gi->setBackground(m_supplementBg);
    connect(gi,SIGNAL(linkActivated(const QString &)),this,SLOT(linkActivated(const QString &)));
    connect(gi,SIGNAL(linkHovered(const QString &)),this,SLOT(linkHovered(const QString &)));
    return gi;
}
/**
 * add the graphics items in m_items to the scene starting with the item
 * at startPos calculating the position as it goes
 *
 * @param startPos
 */
void GraphicsEntry::appendEntries(int startPos) {
  qreal ypos = 0;
  qreal xpos = 0;
  QRectF r;
  QSizeF sz;
  /// calculate the y-position of the last item currently in the scene
  if (startPos > 0) {
    QPointF p = m_items[startPos - 1]->pos();
    r = m_items[startPos - 1]->boundingRect();
    ypos =  p.y() + r.height() + m_entryMargin;
  }
  /// add items updating the ypos as we go
  for(int i=startPos;i < m_items.size();i++) {
    m_items[i]->setPos(xpos,ypos);
    m_scene->addItem(m_items[i]);
    r = m_items[i]->boundingRect();
    if (m_debug) {
      QFileInfo fi(QDir::tempPath(),QString("/tmp/%1.html").arg(m_items[i]->getNode()));
      QFile f(fi.filePath());
      if (f.open(QIODevice::WriteOnly)) {
        QTextStream out(&f);
        out.setCodec("UTF-8");
        out << m_items[i]->toHtml();
      }
    }
    sz = m_items[i]->document()->size();
    //    QGraphicsRectItem * ri = m_scene->addRect(r);
    //    ri->setPos(xpos,ypos);
    //    ri->setPen(QPen(Qt::NoPen));
    //    ri->setBrush(Qt::cyan);
    ypos += sz.height() + m_entryMargin;
  }
}
/**
 * add the graphics items in m_items to the scene starting with the item
 * at startPos
 *
 * recalculates the position of all the items
 *
 * @param startPos
 */
void GraphicsEntry::prependEntries(int startPos) {
  qreal ypos = 0;
  qreal xpos = 0;
  QRectF r;
  QSizeF sz;

  for(int i=0;i < m_items.size();i++) {
    m_items[i]->setPos(xpos,ypos);
    /// check if this is a new item and add it the scene
    if (i < startPos) {
      m_scene->addItem(m_items[i]);
    }
    r = m_items[i]->boundingRect();
    if (m_debug) {
      QFileInfo fi(QDir::tempPath(),QString("/tmp/%1.html").arg(m_items[i]->getNode()));
      QFile f(fi.filePath());
      if (f.open(QIODevice::WriteOnly)) {
        QTextStream out(&f);
        out.setCodec("UTF-8");
        out << m_items[i]->toHtml();
      }
    }
    sz = m_items[i]->document()->size();
    ypos += sz.height() + m_entryMargin;
  }
}
void GraphicsEntry::on_findNode()  {
  qDebug() << "REDUNDANT CODE";
  /*
  QRegExp rx("n\\d+");
  QString node;
  if (rx.indexIn(node) != -1) {
    getXmlForNode(node);
  }
  else {
    getXmlForRoot(node);
    showNode(node);
  }
  */
}
QString GraphicsEntry::transform(const QString & xsl,const QString & xml) {
  //  QString header = "<TEI.2><text><body><div1>";
  //  QString footer = "</div1></body></text></TEI.2>";

  //  QString tei  = header + xml + footer;
  if (m_compiledXsl == 0) {
    std::istringstream iss(xsl.toStdString());
    int r = m_xalan->compileStylesheet(m_xsltSource.toLocal8Bit().data(),m_compiledXsl);
    if (r != 0) {
      QLOG_DEBUG() << "Error compiling stylesheet" << m_xalan->getLastError();
      return QString();
    }
  }
  std::istringstream iss(xml.toStdString());
  std::stringstream ostream;

  m_xalan->transform(iss,m_compiledXsl,ostream);
  return QString::fromStdString(ostream.str());
}
void GraphicsEntry::onZoomIn() {
  m_view->setTransform(m_transform);
  m_scale += .1;
  m_view->scale(m_scale,m_scale);
}
void GraphicsEntry::onZoomOut() {
  m_view->setTransform(m_transform);
  m_scale -= .1;
  m_view->scale(m_scale,m_scale);
}
/**
 * search for the given root in the items list
 *
 * @param root         arabic root to search for
 * @param setFocus     center on the root if found
 * @return index in list of item or -1
 */
int GraphicsEntry::hasRoot(const QString & root,bool setFocus) {
  int max = m_items.size();
  int ix = -1;

  for(int i=0;i < max;i++) {
    if (m_items[i]->isRoot() &&
        (m_items[i]->getRoot() == root)) {
          ix = i;
          i = max;
    }
  }
  qDebug() << Q_FUNC_INFO << root << setFocus << ix;
  if ((ix != -1) && setFocus) {
    m_scene->setFocusItem(m_items[ix]);
    m_view->ensureVisible(m_items[ix]);
    m_currentRoot = root;
    emit(rootChanged(root,QString()));
  }
  return ix;
}
int GraphicsEntry::hasPlace(const Place & p,bool setFocus) {
  int max = m_items.size();
  int ix = -1;
  int supp = p.getSupplement();
  QString root = p.getRoot();
  for(int i=0;i < max;i++) {
    if (m_items[i]->isRoot() &&
        ((m_items[i]->getRoot() == root) &&
         (m_items[i]->getSupplement() == supp))) {
          ix = i;
          i = max;
    }
  }
  qDebug() << Q_FUNC_INFO << root << supp << setFocus << ix;
  if ((ix != -1) && setFocus) {
    m_scene->setFocusItem(m_items[ix]);
    m_view->ensureVisible(m_items[ix]);
    m_place = p;
    m_currentRoot = root;
    emit(rootChanged(root,QString()));
  }
  return ix;
}
QString GraphicsEntry::lastRoot() {
    /// find the last root item
    int ix = m_items.size() - 1;
    QString root;
    while((ix >= 0) && root.isEmpty()) {
      if (m_items[ix]->isRoot()) {
        root = m_items[ix]->getRoot();
      }
      ix--;
    }
    if (root.isEmpty()) {
      QLOG_DEBUG() << "Cannot find root on current page";
    }
    else {
      qDebug() << "emit nextRoo" << root;
      emit nextRoot(root);
    }
    return root;
}
/**
 * find the first root in the current scene
 *
 *
 * @return the text of the root
 */
QString GraphicsEntry::firstRoot() {
  /// find the first root item (should always be the first
  /// element in m_items
  qDebug() << Q_FUNC_INFO;
  int ix = 0;
  int max = m_items.size();
  QString root;
    while((ix < max) && root.isEmpty()) {
      if (m_items[ix]->isRoot()) {
        root = m_items[ix]->getRoot();
      }
      ix++;
    }
    if (root.isEmpty()) {
      QLOG_DEBUG() << "Cannot find root on current page";
    }
    else {
      qDebug() << "emit prevRoot" << root;
      emit prevRoot(root);
    }
    return root;
}
/**
 * find the last root on the page
 * which will emit(nextRoot(<root>))
 */
void GraphicsEntry::nextPageRequested() {
  this->lastRoot();
}

void GraphicsEntry::prevPageRequested() {
  this->firstRoot();
}
void GraphicsEntry::highlight(const QString & target) {
  int ix = -1;
  QTextCursor cursor;
  for(int i=0;i < m_items.size();i++ ) {
    EntryItem * item = m_items[i];
    QTextCursor c = item->highlightRx(target);
    /// get the cursor for the first match
    if (! c.isNull() && (ix == -1)) {
      cursor = c;
      ix = i;
    }
  }
  qDebug() << Q_FUNC_INFO << "item" << ix << "pos" << cursor.position() << cursor.hasSelection();
  /// unselect the text otherwise it will be in the select color
  /// center the view on the word
  if (ix != -1) {
    if ( ! cursor.isNull()) {
      cursor.clearSelection();
      QTextBlock b = cursor.block();
      QTextLayout * layout = b.layout();
      QTextLine line = layout->lineForTextPosition(cursor.position());
      if (line.isValid()) {
        //        qDebug() << "text line pos" << line.position() << line.textLength();
        m_view->centerOn(line.position());
      }
    }
    m_items[ix]->setTextCursor(cursor);
  }
}
