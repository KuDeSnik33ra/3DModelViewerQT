#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>

#include "Camera.h"
#include "Shape.h"

class SceneManager : public QOpenGLWidget, public QOpenGLExtraFunctions
{
    Q_OBJECT

public:
    using QOpenGLWidget::QOpenGLWidget;
    ~SceneManager();

    void enableNavigation(bool value);
    void enableSelection(bool value);

protected:
    void wheelEvent(QWheelEvent* e) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* e) override;
    void timerEvent(QTimerEvent* e) override;

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

public:
    ShapePtr pickShape(const QVector2D& viewportPoint) const;
    ShapePtr pickShape(const Ray& ray) const;
    ShapePtr getShape(const std::string& id) const;
    ShapePtr createShape(const std::string& id);
    void removeShape(const ShapePtr& shape);
    void removeShape(const std::string& id);

private:
    QQuaternion getCameraRotation() const;
    void enableGLDebugLogging();

private:
    QBasicTimer timer;
    QVector2D mousePressPosition;

    Camera m_camera;

    bool m_navigation = true;
    bool m_selection = false;
    QVector3D m_cameraTranslate;
    QVector2D m_cameraRotate;
    QVector2D m_viewportSize;
    QMatrix4x4 m_invProjView;

    std::vector<ShapePtr> m_shapes;
    ShapePtr m_selectedShape;
};