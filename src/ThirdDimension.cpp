#include <iostream>
#include <cmath>
#include <cstdlib>
#include <GLAD\glad.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GLcamera.hpp"

using namespace std;
using namespace glm;

GLuint CLIENTWIDTH = 900;
GLuint CLIENTHEIGHT = 900;

void processInput(GLFWwindow *window,GLfloat* offset,Camera* cam)
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


    const float SPEED = 0.1f;
    
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {;}

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {;}

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {cam->ProcessKeyboard(FORWARD,0.1);;}

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {cam->ProcessKeyboard(BACKWARD,0.1);;}
    
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {cam->ProcessKeyboard(RIGHT,0.1);;}

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {cam->ProcessKeyboard(LEFT,0.1);;}

}
//To do list
/*
(done)Figure a way to calculate the vertices
(done)Figure a way to connect them using indices
(done)Make a shader that can support easier matrix multiplecation
(done)Use an orthogonal projection
(done)Move the camera
(done)learn how to use model space,world space,view space,clip space and screen coordinates
Experiment partial derivatives on the graphing calculatorslc`1 1
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
   // return sqrt(0.4*0.4 - pow(0.6 - sqrt(x*x + y*y),2));
   return x*sin(x*y*0.1);
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



 void FillPoints(GLfloat* points,GLfloat (*func)(GLfloat,GLfloat ) ,unsigned long long n,Vec3 space,Vec3 offset )
{
    
    
  
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

GLfloat* BoundryPoints(vec3 corner,float size)
{
    GLfloat* result = (GLfloat*)calloc(8*3,sizeof(GLfloat));
    int l = 0;
    for (int i = 0;i<2;i++)
    {
        for (int j = 0;j<4;j++)
            {
                
                    result[3*l] = corner.x+i*size;
                    result[3*l+1] = corner.y+j*size;
                    l++;
                
            }
    }
}

GLfloat* BoundryIndices()
{
    GLfloat* result = (GLfloat*)calloc(24,sizeof(GLfloat));

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
    Camera cam(0.0f,f(0,0),0.0f   ,  0.0f,1.0f,0.0f  , 0.0f,0.0f);
    unsigned long long n = 100;

    GLfloat* points = (GLfloat*)calloc(3*n*n,sizeof(GLfloat) );;
    FillPoints(points,f,n,{Space[0],Space[1],Space[2]},{Offset[0],Offset[1],Offset[2]});
    GLuint* indices = FillIndices(n);
    GLfloat xAxis[] = {-1.0f,0.0f,0.0f,
                        1.0f,0.0f,0.0f
                        ,
                        0.0f,-1.0f,0.0f,
                        0.0f,1.0f,0.0f
                        ,
                        0.0f,0.0f,-1.0f,
                        0.0f,0.0f,1.0f};
    
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
         std::cout << vInfo << endl;
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
         std::cout << fInfo << endl;
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
         std::cout << gInfo << endl;
    }

    GLuint VBO;
    GLuint VAO;
    GLuint VEO;

    GLuint VBaxis;
//glGen
//glBindBuffer(GL_ARRAY_BUFFER, VBaxis);
//glBufferData(GL_ARRAY_BUFFER,18,xAxis,GL_STATIC_DRAW);

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
//int Trans = glGetUniformLocation(sProgram , "Translation");
glEnable(GL_DEPTH_TEST);  


float r = 0.4f;
vec3 target(0,0,0);
double yaw = -90.0f;;
double pitch = 0;
mat4 proj = glm::ortho(-1.5f,1.5f, -1.5f,1.5f, -1000.0f,1000.0f );
glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    while (!glfwWindowShouldClose(win))
        {  
            
            if (time + 1/fps < glfwGetTime() )
                {
                    time = glfwGetTime();
                    double x,y;
                    glfwGetCursorPos(win,&x,&y);
                    double dx = x - (float)CLIENTWIDTH/2 ;
                    double dy = y - (float)CLIENTHEIGHT/2 ;
                    
                    cam.ProcessMouseMovement(dx,-dy);
                   

                    glfwSetCursorPos(win,CLIENTWIDTH/2.0f,CLIENTHEIGHT/2.0f);
                    
                    mat4 view = mat4(1.0f);  
                   

                    //Note to self: Make use of the zbuffer to stop overyapping 
                    view = cam.GetViewMatrix();
                    glUniformMatrix4fv(viewLoc , 1 , GL_FALSE , value_ptr(view));
                    glUniformMatrix4fv(projectLoc , 1 , GL_FALSE , value_ptr(proj));
                    
                    processInput(win,Offset,&cam);
                    FillPoints(points,f,n,{Space[0],Space[1],Space[2]},{Offset[0],Offset[1],Offset[2]});
                    glBindBuffer(GL_ARRAY_BUFFER, VBO);
                    glBufferData(GL_ARRAY_BUFFER, 3*n*n*sizeof(GLfloat), points, GL_DYNAMIC_DRAW);
                                    

                } 

                
            glfwPollEvents();
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClear(GL_COLOR_BUFFER_BIT);
            glUseProgram(sProgram);
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 3*2*(n-1)*(n-1) , GL_UNSIGNED_INT, 0);
            glfwSwapBuffers(win);
            
        }
    
}


