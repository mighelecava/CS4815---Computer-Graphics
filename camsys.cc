/**
 * @authors: Nicole Berty (18246702), Michele Cavaliere (18219365),
 *           Sean Lynch (18245137),   Matt Lucey (18247083)
 * To compile: g++ -o camsys camsys.cc -lglut -lGL -lGLU
 */

#include <iostream>
#include <GL/glut.h>	// ==> gl.h and glu.h
#include <Eigen/Geometry>

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::istream;
using std::ostream;

typedef Eigen::Matrix<float, 3, 1> Vector3f;
Eigen::Vector3f eye, at, upVec;
Eigen::Vector3f zc, yc, xc;
float near = 0.0, far = 0.0;
int clipWidth, clipHeight;
int wview = 400, hview = 400;
Eigen::Vector4d p1VC, p2VC, p3VC;

ostream& operator<<(ostream &os, const Eigen::Vector3f& v)
{
  os<< "("<< v.x()<< ", "<< v.y()<< ", "<< v.z()<< ")";
  return os;
}

ostream& operator<<(ostream &os, const Eigen::Vector4d& v)
{
  os<< "("<< v(0)<< ", "<< v(1)<< ", "<< v(2)<< ", "<< v(3)<< ")";
  return os;
}

istream& operator>>(istream &is, Eigen::Vector3f& t)
{
  float x, y, z;
  is>> x>> y>> z;
  t<< x, y, z; //  https://eigen.tuxfamily.org/dox/group__QuickRefPage.html

  return is;
}

void readTriple(Eigen::Vector3f& t)
{
  float x, y, z; 
  cin>> x>> y>> z;

  t<< x, y, z; //  https://eigen.tuxfamily.org/dox/group__QuickRefPage.html
}

void storeQuad(Eigen::Vector4d& v, const Eigen::Vector3f v2)
{
  v[0] = v2[0];
  v[1] = v2[1];
  v[2] = v2[2];
  v[3] = 1;
}

void printVec3d(Eigen::Vector3f &v)
{
  cout << "(" << v[0] << ", " << v[1] << ", " << v[2] << ")" << endl;
}

void printVec(Eigen::Vector4d &v)
{
  cout << "(" << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << ")" << endl;
}

void init (void)
{
  readTriple(eye); //eye
  readTriple(at); //at
  readTriple(upVec); //up  
  
  Eigen::Vector3f temp = eye - at;
  zc = temp / temp.norm();

  Eigen::Vector3f temp2 = upVec.cross(zc);
  xc = temp2 / temp2.norm();

  yc = zc.cross(xc);

  cin >> near >> far;
  cin >> clipWidth >> clipHeight;
}
    
void matrixTransform(Eigen::Vector4d &p)
{
  Eigen::Matrix4d T = Eigen::Matrix4d::Identity(); //translation
      T << 1, 0, 0, -eye[0],
       0, 1, 0, -eye[1],
       0, 0, 1, -eye[2],
       0, 0, 0, 1;

  Eigen::Matrix4d R = Eigen::Matrix4d::Identity(); //rotation
      R << xc[0], xc[1], xc[2], 0,
       yc[0], yc[1], yc[2], 0,
       zc[0], zc[1], zc[2], 0,
       0,     0,     0,     1;
     
  Eigen::Matrix4d viewMat = R*T;
  p = viewMat * p;
}

void matrixProject(Eigen::Vector4d &p)
{
  float width = clipWidth / 2;
  float height = clipHeight / 2;
  float imageAspectRatio = width / height;
//  const float scale = tan(angleOfView * 0.5 * M_PI / 180) * near;
  float scale = 2.5;
  float r = imageAspectRatio * scale;
  float l = -r;
  float t = scale;
  float b = -scale;
  Eigen::Matrix4d P = Eigen::Matrix4d::Identity();
  
 P << ((2 * near) / (r - l)), 0, (r + l) / (r - l), 0,
        0, ( 2 * near)/ (t - b),(t + b)/ (t - b), 0,
        0, 0, -(far + near)/ (far - near), -((2 * (near * far)) / (far - near)),
        0, 0, -1, 0;

  p = P * p; 

}

void query3()
{
  float width = (float) clipWidth / 2.0;
  float height = (float) clipHeight / 2.0;
  float angleW = (float) atan(width / near);
  float angleH = (float) atan(height/near);
  float widthFar = (float) tan(angleW) * far;
  float heightFar = (float) tan(angleH) * far;

  Eigen::Vector3f nearOrigin (0, 0, 0-near);
  Eigen::Vector3f farOrigin (0, 0, 0-far);
  Eigen::Vector3f blf, tlf, trf, brf, blr, tlr, trr, brr;
  
  blf << nearOrigin[0] - width, nearOrigin[1] - height, nearOrigin[2];
  tlf << nearOrigin[0] - width, nearOrigin[1] + height, nearOrigin[2];  
  trf << nearOrigin[0] + width, nearOrigin[1] + height, nearOrigin[2];
  brf << nearOrigin[0] + width, nearOrigin[1] - height, nearOrigin[2];

  blr << farOrigin[0] - widthFar, farOrigin[1] - heightFar, farOrigin[2]; 
  tlr << farOrigin[0] - widthFar, farOrigin[1] + heightFar, farOrigin[2]; 
  trr << farOrigin[0] + widthFar, farOrigin[1] + heightFar, farOrigin[2];
  brr << farOrigin[0] + widthFar, farOrigin[1] - heightFar, farOrigin[2];

  printVec3d(blf);
  printVec3d(tlf);
  printVec3d(trf);
  printVec3d(brf);
  printVec3d(blr);
  printVec3d(tlr);
  printVec3d(trr);
  printVec3d(brr);
}

void query4()
{
  Eigen::Vector3f p1;
  readTriple(p1);
   
  Eigen::Vector4d p1Converted(p1[0],p1[1],p1[2],1);
  matrixTransform(p1Converted);
  printVec(p1Converted);
  
}

void query5()
{
  Eigen::Vector3f p2;
  readTriple(p2);
    
  Eigen::Vector4d p2Converted(p2[0], p2[1], p2[2], 1);
  matrixTransform(p2Converted);
  printVec(p2Converted);
}

void query6()
{
  Eigen::Vector3f p3;
  readTriple(p3);
  Eigen::Vector4d p3Converted(p3[0], p3[1], p3[2], 1);
  matrixTransform(p3Converted);
  matrixProject(p3Converted);
  Eigen::Vector4d p3NDC(p3Converted[0]/p3Converted[3], p3Converted[1]/p3Converted[3], p3Converted[2]/p3Converted[3], p3Converted[3]);
  printVec(p3NDC);
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  glLineWidth(4.0);
  glColor3f(0.0, 1.0, 0.0);
  glBegin(GL_LINE_LOOP);
    glVertex2f(p1VC[0], p1VC[1]);
    glVertex2f(p2VC[0], p2VC[1]);
    glVertex2f(p3VC[0], p3VC[1]);
  glEnd();
  glFlush();
}

void query7()
{
  Eigen::Vector3f p1;
  readTriple(p1);
  Eigen::Vector3f p2;
  readTriple(p2);
  Eigen::Vector3f p3;
  readTriple(p3);
  cin >> wview;
  cin >> hview;
  
  storeQuad(p1VC, p1);
  matrixTransform(p1VC);    
  matrixProject(p1VC); 
  p1VC[2] = p1VC[2] * -3;
  printVec(p1VC);

  
  storeQuad(p2VC, p2);
  matrixTransform(p2VC);
  matrixProject(p2VC); 
  p2VC[2] = p2VC[2] * -3;
  printVec(p2VC);


  storeQuad(p3VC, p3);
  matrixTransform(p3VC);
  matrixProject(p3VC); 
  p3VC[2] = p3VC[2] * -3;
  printVec(p3VC);

      
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(wview, hview);
  glutCreateWindow("camsys");
  glutDisplayFunc(display);
  glutMainLoop();
}

int main(int argc, char** argv)
{
  init();
 
  int query;
  cin >> query;
  if(query == 1) {
      cout<< zc<< endl; 
  } else if (query == 2) {
      cout<< yc<< endl;
  } else if (query == 3) {    
      query3();
  } else if (query == 4) {
      query4();
  } else if (query == 5) {
      query5();
  } else if (query == 6) {
      query6();
  } else if (query == 7) {
    glutInit(&argc, argv);
      query7();
  }
}