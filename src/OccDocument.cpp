#include "OccDocument.h"

#include <AIS_Shape.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <Quantity_Color.hxx>

OccDocument::OccDocument(const QString& name)
  : m_name(name)
{
  Handle(Aspect_DisplayConnection) displayConnection = new Aspect_DisplayConnection();
  m_driver = new OpenGl_GraphicDriver(displayConnection, false);

  m_viewer = new V3d_Viewer(m_driver);
  m_viewer->SetDefaultLights();
  m_viewer->SetLightOn();

  m_context = new AIS_InteractiveContext(m_viewer);
  initDemoScene();
}

void OccDocument::initDemoScene()
{
  // Each document starts with a tiny demo shape.
  const TopoDS_Shape cone = BRepPrimAPI_MakeCone(40.0, 10.0, 70.0).Shape();
  Handle(AIS_Shape) aisCone = new AIS_Shape(cone);
  aisCone->SetColor(Quantity_NOC_STEELBLUE);
  m_context->Display(aisCone, false);
}
