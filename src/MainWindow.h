#pragma once

#include <QMainWindow>

#include <memory>
#include <vector>

class OccView;
class OccDocument;
class QTabBar;
class QWidget;

class MainWindow final : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow() override = default;

private:
  void createMenus();
  void createDocument();
  void closeCurrentDocument();
  void closeDocument(int index);
  void activateDocument(int index);
  void activateNextDocument();
  void syncDocumentTabs();
  void updateWindowCaption();

  QWidget* m_centerPanel = nullptr;
  QTabBar* m_docTabs = nullptr;
  OccView* m_view = nullptr;
  std::vector<std::shared_ptr<OccDocument>> m_documents;
  int m_activeDocument = -1;
};

