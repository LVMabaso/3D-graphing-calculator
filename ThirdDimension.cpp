#include <iostream>
#include <cmath>
#include <cstdlib>
#include <GLAD\glad.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;
GLuint CLIENTWIDTH = 900;
GLuint CLIENTHEIGHT = 900;

void processInput(GLFWwindow *window,GLfloat* offset,GLfloat* cameraPos)
{
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {offset[1]+=10.01;}

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {offset[1]-=10.01;}

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {offset[0]+=10.01;}

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {offset[0]-=10.01;}

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        {offset[2]+=11;}

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        {offset[2]-=11;}


    const float SPEED = 1;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {cameraPos[1]+=SPEED;}

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {cameraPos[1]-=SPEED;}

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {cameraPos[0]+=SPEED;}

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {cameraPos[0]-=SPEED;}
    
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {cameraPos[2]+=SPEED;}

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {cameraPos[2]-=SPEED;}

}
//To do list
/*
(done)Figure a way to calculate the vertices
(done)Figure a way to connect them using indices
(done)Make a shader that can support easier matrix multiplecation
(done)Use an orthogonal projection
(done)Move the camera
(done)learn how to use model space,world space,view space,clip space and screen coordinates
Experiment partial derivatives on the graphing calculator
(done)Use a zbuffer to the prevent overlap thing
Make the camera controllable using mouse movements
Do the CN group project
Do the English presentation
Make a text to function calculator
Learn lighting using shaders
Draw the xy , xz and zy plane
Use gradience for better colours
Use quantanos for better rotation
Make parametric equations possible
Make vector fields possible
End this project in peace
*/
GLfloat f(GLfloat x,GLfloat y)
{
    return sin(x);
}

struct Vec3
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
};

struct Vec2
{
    GLfloat x;
    GLfloat y;
};



GLfloat* FillPoints(GLfloat (*func)(GLfloat,GLfloat ) ,unsigned long long n,Vec3 space,Vec3 offset )
{
    
    
    GLfloat* points = (GLfloat*)calloc(3*n*n,sizeof(GLfloat) );
    GLfloat dx = space.x/n;
    GLfloat dy = space.y/n;
    size_t k = 0;
    for (size_t j = 0;j<n;j++)
    {
        for (size_t i = 0;i<n;i++)
        {
           GLfloat x,y;
            x = i*dx;
            y = j*dy;
            points[k+0] = 2*(x/space.x) - 1;
            points[k+1] = 2*(y/space.y) - 1;
            points[k+2] = 2*(func(offset.x + x,offset.y + y)/space.z) - 1;
//            cout<<"f("<<offset.x + x<<" , "<<offset.y + y<<") = "<<func(offset.x + x,offset.y + y) <<endl;
  //          cout<<endl;
            k+=3;
        }
    }
    

    return points;

}

GLuint* FillIndices(unsigned long long n)
{
    GLuint* indices = (GLuint*)calloc(2*3*(n-1)*(n-1),sizeof(GLfloat));
    size_t k = 0;
    for (size_t j = 0; j<n-1;j++)
    {
        for (size_t i = 0;i<n-1;i++)
        {
            //First Triangle
            indices[k] = j*n+i;//k = i+j*n but this feels simpler somehow
            indices[k+1] = j*n+i+1;
            indices[k+2] = (j+1)*n+i;
            //Second Triangle
            indices[k+3] =  j*n+i+1;
            indices[k+4] = (j+1)*n+i;
            indices[k+5] = (j+1)*n+i+1;
            k+=6;       
        }
    }
    return indices;
}
int main()
{
    glfwInit();
    GLFWwindow* win = glfwCreateWindow(CLIENTWIDTH,CLIENTHEIGHT,"3D Graphing Calculator",NULL,NULL);
    glfwMakeContextCurrent(win);
    glfwWindowHint(GLFW_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    gladLoadGL();
    glViewport(0,0,CLIENTWIDTH,CLIENTHEIGHT);

    GLfloat Space[3] = {45.0,45.0,45.0};
    GLfloat Offset[3] = {-22.5,-22.5,-22.5};
    GLfloat camera[3] = {0,0,-4.0f};
    unsigned long long n = 50;

    GLfloat* points = FillPoints(f,n,{Space[0],Space[1],Space[2]},{Offset[0],Offset[1],Offset[2]});
    GLuint* indices = FillIndices(n);
    
    const GLchar *sCode = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "out vec4 vColor;\n "
    "void main()\n"
    "{\n"
    "   gl_Position = projection * view * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "    vColor = vec4(aPos.xyz,1.0f);\n"
    "}\0";

    const GLchar *fCode =  "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec4 vColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vColor;\n"
    "}\n\0";

    GLuint vShader;
    vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &sCode, NULL);
    glCompileShader(vShader);
    int vsuc;
    char vInfo[512];
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &vsuc);
    if (!vsuc)
    {
        glGetShaderInfoLog(vShader, 512, NULL, vInfo);
        cout << vInfo << endl;
    }

    GLuint fShader; 
    fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fCode, NULL);
    glCompileShader(fShader);
    int fsuc;
    char fInfo[512];
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &fsuc);
    if (!fsuc)
    {
        glGetShaderInfoLog(fShader, 512, NULL, fInfo);
        cout << fInfo << endl;
    }

    GLuint sProgram;
    sProgram = glCreateProgram();
    glAttachShader(sProgram, vShader);
    glAttachShader(sProgram, fShader);
    glLinkProgram(sProgram);

    int gsuc;
    char gInfo[512];
    glGetProgramiv(sProgram, GL_COMPILE_STATUS, &gsuc);
    if (!gsuc)
    {
        glGetProgramInfoLog(sProgram, 512, NULL, gInfo);
        cout << gInfo << endl;
    }

    GLuint VBO;
    GLuint VAO;
    GLuint VEO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VEO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 3*n*n*sizeof(GLfloat), points, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VEO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2*3*(n-1)*(n-1)*sizeof(GLuint), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT,GL_TRUE, 3 * sizeof(GLfloat), (void *)0);

    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    GLfloat time = glfwGetTime();
    double fps = 60;
    /*
    for (int i = 0;i<3*n*n;i++)
        {
            cout<<points[i]<<endl;
            if ((i+1)%3 == 0){cout<<endl;}
        }*/
int viewLoc = glGetUniformLocation(sProgram, "view");
int projectLoc = glGetUniformLocation(sProgram, "projection");
glEnable(GL_DEPTH_TEST);  


float r = 1.0f;
vec3 target(0,0,0);
double yaw = 0;
double pitch = 0;
    while (!glfwWindowShouldClose(win))
        {  
            
            if (time + 1/fps < glfwGetTime() )
                {
                    time = glfwGetTime();
                    double x,y;
                    glfwGetCursorPos(win,&x,&y);
                    double dx = x - (float)CLIENTWIDTH/2 ;
                    double dy = y - (float)CLIENTHEIGHT/2 ;
                    
                    yaw += r*dx;
                    pitch  += r*dy;
                    
                    

                    target.x = sin(radians(yaw))*cos(radians(pitch));
                    target.y = sin(radians(yaw))*sin(radians(pitch));
                    target.z = cos(radians(yaw));

                    target = normalize(target);
                    //target = vec3(0,0,0);
                    glfwSetCursorPos(win,CLIENTWIDTH/2,CLIENTHEIGHT/2);
                    
                    mat4 view = mat4(1.0f);  
                    vec3 cameraPos(camera[0],camera[1],camera[2]);
                    vec3 dir = normalize(cameraPos - target);
                    mat4 proj = glm::ortho(-1.5f,1.5f, -1.5f,1.5f, -1000.0f,1000.0f );
                    //Note to self: Make use of the zbuffer to stop overyapping 
                    view = lookAt(cameraPos , target , vec3(0.0f,1.0f,0.0f));
                    glUniformMatrix4fv(viewLoc , 1 , GL_FALSE , value_ptr(view));
                    glUniformMatrix4fv(projectLoc , 1 , GL_FALSE , value_ptr(proj));
                    
                    processInput(win,Offset,camera);
                    points = FillPoints(f,n,{Space[0],Space[1],Space[2]},{Offset[0],Offset[1],Offset[2]});
                    glBindBuffer(GL_ARRAY_BUFFER, VBO);
                    glBufferData(GL_ARRAY_BUFFER, 3*n*n*sizeof(GLfloat), points, GL_DYNAMIC_DRAW);
                    
                    cout<<"x = "<<target.x<<": y = "<<target.y<<" z = "<<target.z<<endl;                    

                } 

                
            glfwPollEvents();
            glClearColor(0.80085f, 0.21f, 0.7f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClear(GL_COLOR_BUFFER_BIT);
            glUseProgram(sProgram);
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 3*2*(n-1)*(n-1) , GL_UNSIGNED_INT, 0);
            glfwSwapBuffers(win);
            
        }
    
}


