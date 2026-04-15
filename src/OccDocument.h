#pragma once

#include <AIS_InteractiveContext.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <V3d_Viewer.hxx>

#include <QString>

//! Logical OCCT document session (viewer + context + model presentations).
class OccDocument
{
public:
  explicit OccDocument(const QString& name);

  const QString& name() const { return m_name; }
  const Handle(V3d_Viewer)& viewer() const { return m_viewer; }
  const Handle(AIS_InteractiveContext)& context() const { return m_context; }

private:
  void initDemoScene();

private:
  QString m_name;

  Handle(OpenGl_GraphicDriver) m_driver;
  Handle(V3d_Viewer) m_viewer;
  Handle(AIS_InteractiveContext) m_context;
};
