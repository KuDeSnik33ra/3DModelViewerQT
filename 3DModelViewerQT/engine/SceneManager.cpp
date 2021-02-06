#include "SceneManager.h"

#include <QMouseEvent>
#include <QOpenGLDebugLogger>
#include <QLoggingCategory>

SceneManager::~SceneManager()
{
	makeCurrent();
    m_shapes.clear();
	doneCurrent();
}

void SceneManager::enableNavigation(bool value)
{
    m_navigation = value;
}

void SceneManager::enableSelection(bool value)
{
    m_selection = value;
}

void SceneManager::initializeGL()
{
    m_cameraTranslate.setZ(100);
	initializeOpenGLFunctions();

	glClearColor(0, 1, 1, 1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

    GLuint vertexArray = 0;
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    enableGLDebugLogging();

    timer.start(12, this);
}

void SceneManager::resizeGL(int w, int h)
{
    m_viewportSize.setX(w);
    m_viewportSize.setY(h);
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    const float zNear = 0.1f, zFar = 7000.0f, fov = 45.0f;
    m_camera.setPerspective(fov, aspect, zNear, zFar);
}

void SceneManager::paintGL()
{
    QMatrix4x4 rotate;
    rotate.rotate(getCameraRotation());
    QMatrix4x4 translate;
    translate.translate(m_cameraTranslate);

    m_camera.setTransform(translate * rotate);
    m_invProjView = m_camera.getProjView().inverted();

    makeCurrent();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto& shape : m_shapes)
    {
        if (!shape->render(m_camera.getProjView(), shape->getTransformation()))
        {
            continue;
        }
    }
    doneCurrent();
}

ShapePtr SceneManager::pickShape(const QVector2D& viewportPoint) const
{
    QVector3D screenPos((viewportPoint / m_viewportSize - QVector2D(0.5f, 0.5f)) * QVector2D(2.0f, -2.0f), 0.0f);
    QVector3D nearPlanePos = m_invProjView * screenPos;

    Ray ray(m_cameraTranslate, (nearPlanePos - m_cameraTranslate).normalized());
	return pickShape(ray);
}

ShapePtr SceneManager::pickShape(const Ray& ray) const
{
    ShapePtr hitShape;
    std::optional<float> hitDistance;
    for (auto& shape : m_shapes)
    {
        if (auto dst = shape->rayCast(ray))
        {
            if (!hitDistance || (*hitDistance > dst))
            {
                hitShape = shape;
                hitDistance = dst;
            }
        }
    }
    return hitShape;
}

ShapePtr SceneManager::getShape(const std::string& id) const
{
    for (auto shape : m_shapes)
    {
        if (shape->getID() == id)
        {
            return shape;
        }
    }
    return {};
}

ShapePtr SceneManager::createShape(const std::string& id)
{
    auto result = std::make_shared<Shape>(this, id);
    m_shapes.push_back(result);
    return result;
}

void SceneManager::removeShape(const ShapePtr& shape)
{
    for (auto shapeIt = m_shapes.begin(); shapeIt != m_shapes.end(); shapeIt++)
    {
        if ((*shapeIt) == shape)
        {
            m_shapes.erase(shapeIt);
            return;
        }
    }
}

void SceneManager::removeShape(const std::string& id)
{
    for (auto shapeIt = m_shapes.begin(); shapeIt != m_shapes.end(); shapeIt++)
    {
        if ((*shapeIt)->getID() == id)
        {
            m_shapes.erase(shapeIt);
            return;
        }
    }
}

void SceneManager::timerEvent(QTimerEvent*)
{
    update();
}

void SceneManager::wheelEvent(QWheelEvent* e)
{
    if (m_navigation)
    {
        float add = float(e->angleDelta().y()) / 400.0f;
        m_cameraTranslate += getCameraRotation() * QVector3D(0, 0, add);
    }
}

void SceneManager::mouseMoveEvent(QMouseEvent* e)
{
    if (m_navigation && (e->buttons() && Qt::LeftButton))
    {
        QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;
        mousePressPosition = QVector2D(e->localPos());

        m_cameraRotate += diff / 5.0f;
    }
}

void SceneManager::mousePressEvent(QMouseEvent* e)
{
    if (m_selection)
    {
        if (m_selectedShape)
        {
            m_selectedShape->setSelected(false);
        }
        if (auto shape = pickShape(QVector2D(e->localPos())))
        {
            m_selectedShape = shape;
            shape->setSelected(true);
        }
    }
    mousePressPosition = QVector2D(e->localPos());
}

QQuaternion SceneManager::getCameraRotation() const
{
    return QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), m_cameraRotate.x()) * 
        QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), m_cameraRotate.y());
}

void SceneManager::enableGLDebugLogging()
{
#ifdef _DEBUG
    QOpenGLContext* ctx = QOpenGLContext::currentContext();
    QOpenGLDebugLogger* logger = new QOpenGLDebugLogger(this);

    logger->initialize();

    QLoggingCategory::defaultCategory()->setEnabled(QtDebugMsg, true);

    QObject::connect(logger, &QOpenGLDebugLogger::messageLogged, [&](const QOpenGLDebugMessage& debugMessage) {
        qDebug() << debugMessage;
        });
    logger->startLogging();
#endif
}