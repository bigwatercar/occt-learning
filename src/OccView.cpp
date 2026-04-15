#include "OccView.h"

#include "OccDocument.h"

#include <QMouseEvent>
#include <QWheelEvent>

#include <Aspect_ScrollDelta.hxx>
#include <Aspect_VKeyFlags.hxx>
#include <Graphic3d_GraphicDriver.hxx>
#include <Quantity_Color.hxx>
#include <WNT_Window.hxx>

namespace
{
static Aspect_VKeyMouse toOcctMouseButton(Qt::MouseButton b)
{
  switch (b)
  {
    case Qt::LeftButton:   return Aspect_VKeyMouse_LeftButton;
    case Qt::MiddleButton: return Aspect_VKeyMouse_MiddleButton;
    case Qt::RightButton:  return Aspect_VKeyMouse_RightButton;
    default:               return Aspect_VKeyMouse_NONE;
  }
}

static Aspect_VKeyMouse qtButtonsToOcct(Qt::MouseButtons q)
{
  Aspect_VKeyMouse b = Aspect_VKeyMouse_NONE;
  if (q.testFlag(Qt::LeftButton))
    b |= Aspect_VKeyMouse_LeftButton;
  if (q.testFlag(Qt::MiddleButton))
    b |= Aspect_VKeyMouse_MiddleButton;
  if (q.testFlag(Qt::RightButton))
    b |= Aspect_VKeyMouse_RightButton;
  return b;
}

static Aspect_VKeyFlags toOcctModifiers(Qt::KeyboardModifiers mods)
{
  Aspect_VKeyFlags flags = Aspect_VKeyFlags_NONE;
  if (mods.testFlag(Qt::ShiftModifier))   flags |= Aspect_VKeyFlags_SHIFT;
  if (mods.testFlag(Qt::ControlModifier)) flags |= Aspect_VKeyFlags_CTRL;
  if (mods.testFlag(Qt::AltModifier))     flags |= Aspect_VKeyFlags_ALT;
  return flags;
}
} // namespace

OccView::OccView(QWidget* parent)
  : QWidget(parent)
{
  setAttribute(Qt::WA_NoSystemBackground);
  setAttribute(Qt::WA_PaintOnScreen);
  setMouseTracking(true);
  setFocusPolicy(Qt::StrongFocus);
}

OccView::~OccView() = default;

void OccView::setDocument(const std::shared_ptr<OccDocument>& doc)
{
  if (m_document == doc)
    return;

  m_document = doc;
  m_view.Nullify();
  m_window.Nullify();
  m_controller.ResetViewInput();
  ensureViewBound();
  update();
}

const Handle(AIS_InteractiveContext)& OccView::context() const
{
  static Handle(AIS_InteractiveContext) nullCtx;
  return m_document ? m_document->context() : nullCtx;
}

void OccView::fitAll()
{
  if (m_view.IsNull())
    return;

  m_view->FitAll(0.01, false);
  update();
}

QPaintEngine* OccView::paintEngine() const
{
  return nullptr; // required for native OpenGL drawing by OCCT
}

void OccView::showEvent(QShowEvent* e)
{
  QWidget::showEvent(e);
  ensureViewBound();
}

void OccView::resizeEvent(QResizeEvent* e)
{
  QWidget::resizeEvent(e);
  if (m_view.IsNull())
    return;

  m_view->MustBeResized();
  update();
}

void OccView::paintEvent(QPaintEvent* e)
{
  Q_UNUSED(e);
  if (m_document == nullptr)
    return;

  ensureViewBound();
  if (m_view.IsNull())
    return;

  m_controller.FlushViewEvents(m_document->context(), m_view, Standard_True);
  m_view->Redraw();
}

void OccView::mousePressEvent(QMouseEvent* e)
{
  if (m_view.IsNull())
    return;

  const auto btn = toOcctMouseButton(e->button());
  const auto mods = toOcctModifiers(e->modifiers());
  m_controller.PressMouseButton(Graphic3d_Vec2i(e->pos().x(), e->pos().y()), btn, mods, false);
  update();
}

void OccView::mouseReleaseEvent(QMouseEvent* e)
{
  if (m_view.IsNull())
    return;

  const auto btn = toOcctMouseButton(e->button());
  const auto mods = toOcctModifiers(e->modifiers());
  m_controller.ReleaseMouseButton(Graphic3d_Vec2i(e->pos().x(), e->pos().y()), btn, mods, false);
  update();
}

void OccView::mouseMoveEvent(QMouseEvent* e)
{
  if (m_view.IsNull())
    return;

  const auto mods = toOcctModifiers(e->modifiers());
  m_controller.UpdateMousePosition(Graphic3d_Vec2i(e->pos().x(), e->pos().y()),
                                 qtButtonsToOcct(e->buttons()),
                                 mods,
                                 false);
  update();
}

void OccView::wheelEvent(QWheelEvent* e)
{
  if (m_view.IsNull())
    return;

  const QPoint angle = e->angleDelta();
  if (angle.isNull())
    return;

  const auto mods = toOcctModifiers(e->modifiers());
  const NCollection_Vec2<int> pnt(static_cast<int>(e->position().x()),
                                  static_cast<int>(e->position().y()));
  m_controller.UpdateMouseScroll(Aspect_ScrollDelta(pnt, static_cast<Standard_Real>(angle.y()), mods));
  e->accept();
  update();
}

void OccView::ensureViewBound()
{
  if (m_document == nullptr || !m_view.IsNull())
    return;

  // Bind current QWidget native window handle to a fresh view from the active document.
  const auto hwnd = reinterpret_cast<HWND>(winId());
  m_window = new WNT_Window(hwnd);

  m_view = m_document->viewer()->CreateView();
  m_view->SetWindow(m_window);
  if (!m_window->IsMapped())
  {
    m_window->Map();
  }

  m_view->SetBackgroundColor(Quantity_Color(0.12, 0.12, 0.14, Quantity_TOC_RGB));
  m_view->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GRAY80, 0.08, V3d_ZBUFFER);

  // Default to "CAD-like": rotate with LMB, pan with MMB, zoom with wheel / RMB.
  m_controller.SetNavigationMode(AIS_NavigationMode_Orbit);

  m_view->MustBeResized();
  m_view->FitAll(0.01, false);
  m_view->Redraw();
}

