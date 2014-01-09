#ifndef GLCAMERANODE_H
#define GLCAMERANODE_H
#include "geometry.h"
#include "scenegraphnode.h"


class GLCameraNode : public SceneGraphNode
{
public:
    GLCameraNode(QObject *parent, glm::mat4 transform, float near, float far, float fov, QWindow *window);
    Geometry::Ray worldRayAtDisplayPosition(float pixelX, float pixelY);

    void calculateVPMatrix();
    glm::mat4 viewMatrix() const;
    glm::mat4 projectionMatrix() const;
    glm::mat4 viewProjectionMatrix() const;

    //normalized viewport within a given window,
    //all numerical arguments range from 0 to 1 and are multiplied by the size of the window in all getters and before being passed to OpenGL calls
    class GLViewPort{
    public:
        QWindow *m_window;
        GLViewPort(float offsetX, float offsetY, float width, float height, QWindow *window);

        float offsetX() const;
        float offsetY() const;
        float width() const;
        float height() const;

        //calls glViewport with parameters
        void set() const;
        //
        glm::vec2 displayCoordsToViewportCoords(float pixelX, float pixelY) const;

    private:
        float m_offsetX, m_offsetY, m_width, m_height;
    };

    GLViewPort viewport() const;
    void setViewport(const GLViewPort &viewport);

private:
    float near, far, fov;
    GLViewPort m_viewport;
    glm::mat4 m_viewMatrix, m_projectionMatrix, m_viewProjectionMatrix;

};


#endif // GLCAMERANODE_H
