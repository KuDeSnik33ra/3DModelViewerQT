#include "MainWindow.h"
#include "ui_mainwindow.h"

#include <random>

#include <QLabel>
#include <QLayout>

std::default_random_engine* getRandomGen()
{
    static __declspec(thread) std::default_random_engine* generator = nullptr;
    if (!generator)
        generator = new std::default_random_engine;
    return generator;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto centerLayout = new QGridLayout(ui->centralwidget);
#ifndef QT_NO_OPENGL
    auto glWidget = new SceneManager(ui->centralwidget);
    centerLayout->addWidget(glWidget);

    ui->menubar->addAction("Create textured cube", [this, glWidget](bool)
        {
            if (auto material = createTexturedMaterial())
            {
                auto shape = glWidget->createShape(std::to_string(m_counter++));
                if (!shape->createCube(std::uniform_int_distribution<int>(1, 20)(*getRandomGen())))
                {
                    glWidget->removeShape(shape);
                    return;
                }
                shape->setMaterial(material);

                std::uniform_int_distribution<int> posDistribution(-50, 50);

                QMatrix4x4 transform;
                transform.translate(QVector3D(
                    posDistribution(*getRandomGen()),
                    posDistribution(*getRandomGen()),
                    posDistribution(*getRandomGen())
                ));
                shape->setTransform(transform);
            }
        }
    );

    ui->menubar->addAction("Create colored cube", [this, glWidget](bool)
        {
            std::uniform_int_distribution<int> colorDistr(0, 255);
            if (auto material = createColoredMaterial(QColor(colorDistr(*getRandomGen()), 
                colorDistr(*getRandomGen()), colorDistr(*getRandomGen()), 255)))
            {
                auto shape = glWidget->createShape(std::to_string(m_counter++));
                if (!shape->createCube(std::uniform_int_distribution<int>(1, 20)(*getRandomGen())))
                {
                    glWidget->removeShape(shape);
                    return;
                }
                shape->setMaterial(material);

                std::uniform_int_distribution<int> posDistribution(-50, 50);

                QMatrix4x4 transform;
                transform.translate(QVector3D(
                    posDistribution(*getRandomGen()),
                    posDistribution(*getRandomGen()),
                    posDistribution(*getRandomGen())
                ));
                shape->setTransform(transform);
            }
        }
    );

    ui->menubar->addSeparator();
    ui->menubar->addSeparator();

    auto navAction = ui->menubar->addAction("Navigation", [this, glWidget](bool state)
        {
            glWidget->enableNavigation(state);
            glWidget->enableSelection(!state);
        }
    );
    navAction->setCheckable(true);
    navAction->setChecked(true);

    auto selectAction = ui->menubar->addAction("Selection", [this, glWidget](bool state)
        {
            glWidget->enableNavigation(!state);
            glWidget->enableSelection(state);
        }
    );
    selectAction->setCheckable(true);
    selectAction->setChecked(false);

    auto mouseActions = new QActionGroup(this);
    mouseActions->addAction(navAction);
    mouseActions->addAction(selectAction);
    mouseActions->setExclusive(true);

#else
    auto noglWidget = new QLabel("OpenGL Support required", ui->centralwidget);
    centerLayout->addWidget(noglWidget);
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
}

MaterialPtr MainWindow::createTexturedMaterial() const
{
    auto material = std::make_shared<Material>();
    if (!material->setVertexShaderFromSourceFile(":/vshader.glsl"))
    {
        return {};
    }
    if (!material->setFragmentShaderFromSourceFile(":/fshaderTexture.glsl"))
    {
        return {};
    }
    if (!material->setTexture(":/texture0.jpg"))
    {
        return {};
    }
    return material;
}

MaterialPtr MainWindow::createColoredMaterial(const QColor& color) const
{
    auto material = std::make_shared<Material>();
    if (!material->setVertexShaderFromSourceFile(":/vshader.glsl"))
    {
        return {};
    }
    if (!material->setFragmentShaderFromSourceFile(":/fshaderColor.glsl"))
    {
        return {};
    }
    material->setColor(color);
    return material;
}