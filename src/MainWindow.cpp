#include "MainWindow.h"

#include <algorithm>

#include <QMenu>
#include <QMenuBar>
#include <QSignalBlocker>
#include <QStatusBar>
#include <QTabBar>
#include <QVBoxLayout>
#include <QWidget>

#include "OccDocument.h"
#include "OccView.h"

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
{
  createMenus();
  m_centerPanel = new QWidget(this);
  auto* layout = new QVBoxLayout(m_centerPanel);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  m_docTabs = new QTabBar(m_centerPanel);
  m_docTabs->setExpanding(false);
  m_docTabs->setMovable(false);
  m_docTabs->setTabsClosable(true);
  layout->addWidget(m_docTabs);

  m_view = new OccView(m_centerPanel);
  layout->addWidget(m_view, 1);
  setCentralWidget(m_centerPanel);

  connect(m_docTabs, &QTabBar::currentChanged, this, [this](int index) {
    activateDocument(index);
  });
  connect(m_docTabs, &QTabBar::tabCloseRequested, this, [this](int index) {
    closeDocument(index);
  });

  createDocument();
  statusBar()->showMessage(QStringLiteral("Ready"), 1500);
}

void MainWindow::createMenus()
{
  auto* fileMenu = menuBar()->addMenu(QStringLiteral("&File"));
  fileMenu->addAction(QStringLiteral("New Document"), this, [this]() { createDocument(); });
  fileMenu->addAction(QStringLiteral("Close Document"), this, [this]() { closeCurrentDocument(); });
  fileMenu->addAction(QStringLiteral("Exit"), this, &QWidget::close);

  auto* viewMenu = menuBar()->addMenu(QStringLiteral("&View"));
  viewMenu->addAction(QStringLiteral("Fit All"), this, [this]() { m_view->fitAll(); });
  viewMenu->addAction(QStringLiteral("Next Document"), this, [this]() { activateNextDocument(); });

  auto* helpMenu = menuBar()->addMenu(QStringLiteral("&Help"));
  helpMenu->addAction(QStringLiteral("About"));
}

void MainWindow::createDocument()
{
  const int nextId = static_cast<int>(m_documents.size()) + 1;
  auto doc = std::make_shared<OccDocument>(QStringLiteral("Document %1").arg(nextId));
  m_documents.push_back(std::move(doc));
  activateDocument(static_cast<int>(m_documents.size()) - 1);
}

void MainWindow::closeCurrentDocument()
{
  closeDocument(m_activeDocument);
}

void MainWindow::closeDocument(int index)
{
  if (index < 0 || index >= static_cast<int>(m_documents.size()))
    return;

  m_documents.erase(m_documents.begin() + index);
  if (m_documents.empty())
  {
    m_activeDocument = -1;
    m_view->setDocument(nullptr);
    syncDocumentTabs();
    updateWindowCaption();
    return;
  }

  const int next = std::min(index, static_cast<int>(m_documents.size()) - 1);
  activateDocument(next);
}

void MainWindow::activateDocument(int index)
{
  if (index < 0 || index >= static_cast<int>(m_documents.size()))
    return;

  m_activeDocument = index;
  m_view->setDocument(m_documents[static_cast<size_t>(index)]);
  m_view->fitAll();
  syncDocumentTabs();
  updateWindowCaption();
  statusBar()->showMessage(QStringLiteral("Active: %1").arg(m_documents[static_cast<size_t>(index)]->name()), 2000);
}

void MainWindow::activateNextDocument()
{
  if (m_documents.empty())
    return;

  const int next = (m_activeDocument + 1) % static_cast<int>(m_documents.size());
  activateDocument(next);
}

void MainWindow::syncDocumentTabs()
{
  if (m_docTabs == nullptr)
    return;

  QSignalBlocker blocker(m_docTabs);
  while (m_docTabs->count() > 0)
  {
    m_docTabs->removeTab(0);
  }

  for (const auto& doc : m_documents)
  {
    m_docTabs->addTab(doc->name());
  }

  m_docTabs->setVisible(!m_documents.empty());
  if (m_activeDocument >= 0 && m_activeDocument < m_docTabs->count())
  {
    m_docTabs->setCurrentIndex(m_activeDocument);
  }
}

void MainWindow::updateWindowCaption()
{
  if (m_activeDocument < 0 || m_activeDocument >= static_cast<int>(m_documents.size()))
  {
    setWindowTitle(QStringLiteral("OCCT 7.9.3 + Qt5 Multi-Document Scaffold"));
    return;
  }

  const auto& docName = m_documents[static_cast<size_t>(m_activeDocument)]->name();
  setWindowTitle(QStringLiteral("OCCT 7.9.3 + Qt5 Multi-Document Scaffold - %1").arg(docName));
}

