#pragma once

#include <QWidget>

#include <AIS_ViewController.hxx>
#include <Aspect_Window.hxx>
#include <V3d_View.hxx>

#include <memory>

class AIS_InteractiveContext;
class OccDocument;

class OccView final : public QWidget
{
  Q_OBJECT

public:
  explicit OccView(QWidget* parent = nullptr);
  ~OccView() override;

  void setDocument(const std::shared_ptr<OccDocument>& doc);
  std::shared_ptr<OccDocument> document() const { return m_document; }

  const Handle(AIS_InteractiveContext)& context() const;
  const Handle(V3d_View)& view() const { return m_view; }
  void fitAll();

protected:
  QPaintEngine* paintEngine() const override;

  void showEvent(QShowEvent* e) override;
  void resizeEvent(QResizeEvent* e) override;
  void paintEvent(QPaintEvent* e) override;

  void mousePressEvent(QMouseEvent* e) override;
  void mouseReleaseEvent(QMouseEvent* e) override;
  void mouseMoveEvent(QMouseEvent* e) override;
  void wheelEvent(QWheelEvent* e) override;

private:
  void ensureViewBound();

private:
  std::shared_ptr<OccDocument> m_document;
  Handle(V3d_View) m_view;
  Handle(Aspect_Window) m_window;
  AIS_ViewController m_controller;
};

