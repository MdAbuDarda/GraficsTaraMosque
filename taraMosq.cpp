#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<windows.h>

#include<GL/glut.h>

double cameraAngle, cameraAngleDelta;
int canDrawGrid;

double cameraHeight;	
double cameraRadius;

int num_texture  = -1;
GLuint domimg,cylimg,grassimg,wallimg,taraimg,floorimg,skyimg,vfloorimg,tryimg,doorimg ,gumbojimg,upvfloorimg,checkimg,groundfimg;

int LoadBitmapImage(char *filename)
{
    int i, j=0;
    FILE *l_file;
    unsigned char *l_texture;

    BITMAPFILEHEADER fileheader;
    BITMAPINFOHEADER infoheader;
    RGBTRIPLE rgb;

    num_texture++;

    if( (l_file = fopen(filename, "rb"))==NULL) return (-1);

    fread(&fileheader, sizeof(fileheader), 1, l_file);

    fseek(l_file, sizeof(fileheader), SEEK_SET);
    fread(&infoheader, sizeof(infoheader), 1, l_file);

    l_texture = (byte *) malloc(infoheader.biWidth * infoheader.biHeight * 4);
    memset(l_texture, 0, infoheader.biWidth * infoheader.biHeight * 4);
	for (i=0; i < infoheader.biWidth*infoheader.biHeight; i++)
		{
				fread(&rgb, sizeof(rgb), 1, l_file);

				l_texture[j+0] = rgb.rgbtRed;
				l_texture[j+1] = rgb.rgbtGreen;
				l_texture[j+2] = rgb.rgbtBlue;
				l_texture[j+3] = 255;
				j += 4;
		}
    fclose(l_file);

    glBindTexture(GL_TEXTURE_2D, num_texture);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

// glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, infoheader.biWidth, infoheader.biHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, l_texture);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 4, infoheader.biWidth, infoheader.biHeight, GL_RGBA, GL_UNSIGNED_BYTE, l_texture);

    free(l_texture);

    return (num_texture);

}

void loadImage()
{
	checkimg = LoadBitmapImage("image/check.bmp");
	domimg = LoadBitmapImage("image/dome.bmp");/// varanda boder
	cylimg = LoadBitmapImage("image/gumboj.bmp");
    grassimg = LoadBitmapImage("image/grass.bmp");
	floorimg = LoadBitmapImage("image/floor.bmp");
	skyimg = LoadBitmapImage("image/sky.bmp");
	vfloorimg = LoadBitmapImage("image/vfloor.bmp");
	wallimg = LoadBitmapImage("image/wall.bmp");
	tryimg = LoadBitmapImage("image/try.bmp");
    doorimg = LoadBitmapImage("image/door.bmp");
    gumbojimg = LoadBitmapImage("image/gumboj.bmp");
	groundfimg = LoadBitmapImage("image/groundf.bmp");
    upvfloorimg = LoadBitmapImage("image/upvfloor.bmp");
	printf("Load successful");
}
void display(){
	//codes for Models, Camera
	
	//clear the display
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		//clear buffers to preset values

	/***************************
	/ set-up camera (view) here
	****************************/ 
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);		//specify which matrix is the current matrix

	//initialize the matrix
	glLoadIdentity();				//replace the current matrix with the identity matrix [Diagonals have 1, others have 0]

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(0,-150,20,	0,0,0,	0,0,1);
	gluLookAt(cameraRadius*sin(cameraAngle), -cameraRadius*cos(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	
	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/**************************************************
	/ Grid and axes Lines(You can remove them if u want)
	***************************************************/
	// draw the three major AXES
	glBegin(GL_LINES);
		//X axis
		glColor3f(0, 1, 0);	//100% Green
		glVertex3f(-150, 0, 0);
		glVertex3f( 150, 0, 0);
		
		//Y axis
		glColor3f(0, 0, 1);	//100% Blue
		glVertex3f(0, -150, 0);	// intentionally extended to -150 to 150, no big deal
		glVertex3f(0,  150, 0);
		
		//Z axis
		glColor3f(1, 1, 1);	//100% White
		glVertex3f( 0, 0, -150);
		glVertex3f(0, 0, 150);
	glEnd();

	//some gridlines along the field
	int i;
	if(canDrawGrid){
	glColor3f(0.5, 0.5, 0.5);	//grey
		glBegin(GL_LINES);
			for(i=-10;i<=10;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -100, 0);
				glVertex3f(i*10,  100, 0);

				//lines parallel to X-axis
				glVertex3f(-100, i*10, 0);
				glVertex3f( 100, i*10, 0);
			}
		glEnd();
	}
	
	
	


	/****************************
	/ Add your objects from here
	****************************/
////// start of wall//////////


	//front right side wall
	glPushMatrix();{
	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, tryimg);
	glBegin(GL_QUADS);{
		glTexCoord2f(0,1);
		glVertex3f(0,0,60);
		glTexCoord2f(0,0);
		glVertex3f(100,0,60);
		glTexCoord2f(1,0);
		glVertex3f(100,0,0);
		glTexCoord2f(1,1);
		glVertex3f(0,0,0);
	}glEnd();
}glDisable(GL_TEXTURE_2D);
	}glPopMatrix() ; 
	
	//front left side wall
	
	glPushMatrix();{
	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, tryimg);
	glBegin(GL_QUADS);{
		glTexCoord2f(0,1);
		glVertex3f(0,0,60);
		glTexCoord2f(0,0);
		glVertex3f(-100,0,60);
		glTexCoord2f(1,0);
		glVertex3f(-100,0,0);
		glTexCoord2f(1,1);
		glVertex3f(0,0,0);
	}glEnd();
 }glDisable(GL_TEXTURE_2D);
	}glPopMatrix();
	
	// back right side wall
	glPushMatrix();{
	glColor3f(1,1,1);
	glTranslatef(0,99,0);
	glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, wallimg);
	glBegin(GL_QUADS);{
		glTexCoord2f(0,1);
		glVertex3f(0,0,60);
		glTexCoord2f(0,0);
		glVertex3f(100,0,60);
		glTexCoord2f(1,0);
		glVertex3f(100,0,0);
		glTexCoord2f(1,1);
		glVertex3f(0,0,0);
	}glEnd();
  }glDisable(GL_TEXTURE_2D);
	}glPopMatrix();

	//back left side wall
	glPushMatrix();{
	glColor3f(1,1,1);
	glTranslatef(0,99,0);
	glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, wallimg);
	glBegin(GL_QUADS);{
		glTexCoord2f(0,1);
		glVertex3f(0,0,60);
		glTexCoord2f(0,0);
		glVertex3f(-100,0,60);
		glTexCoord2f(1,0);
		glVertex3f(-100,0,0);
		glTexCoord2f(1,1);
		glVertex3f(0,0,0);
	}glEnd();
  }glDisable(GL_TEXTURE_2D);
	}glPopMatrix();

// right side wall
	glPushMatrix();{
	glColor3f(1,1,1);
	glRotatef(270,0,0,1);
	glTranslatef(0,100,0);
	glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, wallimg);
	glBegin(GL_QUADS);{
		glTexCoord2f(0,1);
		glVertex3f(0,0,60);
		glTexCoord2f(0,0);
		glVertex3f(-100,0,60);
		glTexCoord2f(1,0);
		glVertex3f(-100,0,0);
		glTexCoord2f(1,1);
		glVertex3f(0,0,0);
	}glEnd();
  }glDisable(GL_TEXTURE_2D);
	}glPopMatrix();

// left side wall
	glPushMatrix();{
	glColor3f(1,1,1);
	glRotatef(270,0,0,1);
	glTranslatef(0,-100,0);
	glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, wallimg);
	glBegin(GL_QUADS);{
		glTexCoord2f(0,1);
		glVertex3f(0,0,60);
		glTexCoord2f(0,0);
		glVertex3f(-100,0,60);
		glTexCoord2f(1,0);
		glVertex3f(-100,0,0);
		glTexCoord2f(1,1);
		glVertex3f(0,0,0);
	}glEnd();
  }glDisable(GL_TEXTURE_2D);
	}glPopMatrix();


	//upper right side wall
	glPushMatrix();{
glColor3f(1,1,1);
	glTranslatef(0,0,60.1);
	glRotatef(270,1,0,0);
	glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, upvfloorimg);
	glBegin(GL_QUADS);{
		glTexCoord2f(0,1);
		glVertex3f(0,0,100);
		glTexCoord2f(0,0);
		glVertex3f(100,0,100);
		glTexCoord2f(1,0);
		glVertex3f(100,0,0);
		glTexCoord2f(1,1);
		glVertex3f(0,0,0);
	}glEnd();
   }glDisable(GL_TEXTURE_2D);
}glPopMatrix();


	//upper left side wall

	
	glPushMatrix();{
	glColor3f(1,1,1);
	glTranslatef(0,0,60.1);
	glRotatef(270,1,0,0);
	glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, upvfloorimg);
	glBegin(GL_QUADS);{
		glTexCoord2f(0,1);
		glVertex3f(0,0,100);
		glTexCoord2f(0,0);
		glVertex3f(-100,0,100);
		glTexCoord2f(1,0);
		glVertex3f(-100,0,0);
		glTexCoord2f(1,1);
		glVertex3f(0,0,0);
	}glEnd();
  }glDisable(GL_TEXTURE_2D);
}glPopMatrix();


///// end of wall///////////////

/////  start of door////

//front middle side wall door
	glPushMatrix();{

		// door curve1
glPushMatrix();{
	double equ[4];
		equ[0]=0;
		equ[1]=1;
		equ[2]=0;
		equ[3]=0;
		glTranslatef(0,-0.3,38);
		glRotatef(-270,1,0,0);
	glColor3f(.6,0.2,0);
	glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				glutSolidTorus(1.5,12,25,25);

			}glDisable(GL_CLIP_PLANE0);
}glPopMatrix();
//cube left
		glPushMatrix();{
		glColor3f(.6,0.2,0);
	glTranslatef(-13,-0.3,20);
	glScalef(2,2,36);
	glutSolidCube(1);
	}glPopMatrix();
//cube right
		glPushMatrix();{
		glColor3f(.6,0.2,0);
	glTranslatef(13,-0.3,20);
	glScalef(2,2,36);
	glutSolidCube(1);
	}glPopMatrix();

//cube down
		glPushMatrix();{
		glColor3f(.6,0.2,0);
		glTranslatef(0,-0.3,2);
		glRotatef(270,0,0,1);
	
	glScalef(2,27,2);
	glutSolidCube(1);
	}glPopMatrix();

//cube up
		glPushMatrix();{
		glColor3f(.6,0.2,0);
		glTranslatef(0,-0.3,37);
		glRotatef(270,0,0,1);
	glScalef(2,25,2);
	glutSolidCube(1);
	}glPopMatrix();
// door square
	glColor3f(1,1,1);
	glTranslatef(-12.5,-0.3,2);
	glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, doorimg);
	glBegin(GL_QUADS);{
		glTexCoord2f(0,1);
		glVertex3f(0,0,35);
		glTexCoord2f(0,0);
		glVertex3f(25,0,35);
		glTexCoord2f(1,0);
		glVertex3f(25,0,0);
		glTexCoord2f(1,1);
		glVertex3f(0,0,0);
	}glEnd();
}glDisable(GL_TEXTURE_2D);
	}glPopMatrix();
	


//front left side wall door02
	glPushMatrix();{

		// door curve1
glPushMatrix();{
	double equ[4];
		equ[0]=0;
		equ[1]=1;
		equ[2]=0;
		equ[3]=0;
		glTranslatef(-77.8,-0.3,38);
		glRotatef(-270,1,0,0);
	glColor3f(.6,0.2,0);
	glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
		    glutSolidTorus(1.5,12,25,25);

			}glDisable(GL_CLIP_PLANE0);
}glPopMatrix();
//cube left
		glPushMatrix();{
		glColor3f(.6,0.2,0);
	glTranslatef(-91,-0.3,20);
	glScalef(2,2,36);
	glutSolidCube(1);
	}glPopMatrix();
//cube right
		glPushMatrix();{
		glColor3f(.6,0.2,0);
	glTranslatef(-66,-0.3,20);
	glScalef(2,2,36);
	glutSolidCube(1);
	}glPopMatrix();

//cube down
		glPushMatrix();{
		glColor3f(.6,0.2,0);
		glTranslatef(-77.5,-0.3,2);
		glRotatef(270,0,0,1);
	
	glScalef(2,27,2);
	glutSolidCube(1);
	}glPopMatrix();

//cube up
		glPushMatrix();{
		glColor3f(.6,0.2,0);
		glTranslatef(-77.5,-0.3,37);
		glRotatef(270,0,0,1);
	glScalef(2,25,2);
	glutSolidCube(1);
	}glPopMatrix();
// door square
	glColor3f(1,1,1);
	glTranslatef(-90,-0.3,2);
	glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, doorimg);
	glBegin(GL_QUADS);{
		glTexCoord2f(0,1);
		glVertex3f(0,0,35);
		glTexCoord2f(0,0);
		glVertex3f(25,0,35);
		glTexCoord2f(1,0);
		glVertex3f(25,0,0);
		glTexCoord2f(1,1);
		glVertex3f(0,0,0);
	}glEnd();
}glDisable(GL_TEXTURE_2D);
	}glPopMatrix();


//front right side wall door03
	glPushMatrix();{

		// door curve1
glPushMatrix();{
	double equ[4];
		equ[0]=0;
		equ[1]=1;
		equ[2]=0;
		equ[3]=0;
		glTranslatef(77.8,-0.3,38);
		glRotatef(-270,1,0,0);
	glColor3f(.6,0.2,0);
	glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				glutSolidTorus(1.5,12,25,25);

			}glDisable(GL_CLIP_PLANE0);
}glPopMatrix();
//cube left
		glPushMatrix();{
		glColor3f(.6,0.2,0);
	glTranslatef(91,-0.3,20);
	glScalef(2,2,36);
	glutSolidCube(1);
	}glPopMatrix();
//cube right
		glPushMatrix();{
		glColor3f(.6,0.2,0);
	glTranslatef(66,-0.3,20);
	glScalef(2,2,36);
	glutSolidCube(1);
	}glPopMatrix();

//cube down
		glPushMatrix();{
		glColor3f(.6,0.2,0);
		glTranslatef(78.5,-0.3,2);
		glRotatef(270,0,0,1);
	
	glScalef(2,27,2);
	glutSolidCube(1);
	}glPopMatrix();

//cube up
		glPushMatrix();{
		glColor3f(.6,0.2,0);
		glTranslatef(77.5,-0.3,37);
		glRotatef(270,0,0,1);
	glScalef(2,25,2);
	glutSolidCube(1);
	}glPopMatrix();
// door square
	glColor3f(1,1,1);
	glTranslatef(65,-0.3,2);
	glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, doorimg);
	glBegin(GL_QUADS);{
		glTexCoord2f(0,1);
		glVertex3f(0,0,35);
		glTexCoord2f(0,0);
		glVertex3f(25,0,35);
		glTexCoord2f(1,0);
		glVertex3f(25,0,0);
		glTexCoord2f(1,1);
		glVertex3f(0,0,0);
	}glEnd();
}glDisable(GL_TEXTURE_2D);
	}glPopMatrix();


///////end of door/////////



//////  start of varanda/////////////


//varanda roof right
	glPushMatrix();{
	glColor3f(1,1,1);
	glTranslatef(0,-50,60.1);
	glRotatef(270,1,0,0);
	glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, upvfloorimg);
	glBegin(GL_QUADS);{
		glTexCoord2f(0,1);
		glVertex3f(0,0,50);
		glTexCoord2f(0,0);
		glVertex3f(100,0,50);
		glTexCoord2f(1,0);
		glVertex3f(100,0,0);
		glTexCoord2f(1,1);
		glVertex3f(0,0,0);
	}glEnd();
   }glDisable(GL_TEXTURE_2D);
}glPopMatrix();


//varanda left roof
	glPushMatrix();{
	glColor3f(1,1,1);
	glTranslatef(0,-50,60.1);
	glRotatef(270,1,0,0);
	glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, upvfloorimg);
	glBegin(GL_QUADS);{
		glTexCoord2f(0,1);
		glVertex3f(0,0,50);
		glTexCoord2f(0,0);
		glVertex3f(-100,0,50);
		glTexCoord2f(1,0);
		glVertex3f(-100,0,0);
		glTexCoord2f(1,1);
		glVertex3f(0,0,0);
	}glEnd();
   }glDisable(GL_TEXTURE_2D);
}glPopMatrix();  

//varanda front right
	glPushMatrix();{
	//glColor3f(1,.2,.8);
		glColor3f(1,1,1);
	glTranslatef(0,-50,45);
	glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, domimg);
	glBegin(GL_QUADS);{
		glTexCoord2f(0,1);
		glVertex3f(0,0,15);
		glTexCoord2f(0,0);
		glVertex3f(100,0,15);
		glTexCoord2f(1,0);
		glVertex3f(100,0,0);
		glTexCoord2f(1,1);
		glVertex3f(0,0,0);
	}glEnd();
}glDisable(GL_TEXTURE_2D);
	}glPopMatrix(); 

//varanda front left
	glPushMatrix();{
	//glColor3f(1,.2,.8);
		glColor3f(1,1,1);
	glTranslatef(0,-50,45);
	glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, domimg);
	glBegin(GL_QUADS);{
		glTexCoord2f(0,1);
		glVertex3f(0,0,15);
		glTexCoord2f(0,0);
		glVertex3f(-100,0,15);
		glTexCoord2f(1,0);
		glVertex3f(-100,0,0);
		glTexCoord2f(1,1);
		glVertex3f(0,0,0);
	}glEnd();
}glDisable(GL_TEXTURE_2D);
	}glPopMatrix();
	
// varanda front right side
	glPushMatrix();{
	//glColor3f(1,.2,.8);
		glColor3f(1,1,1);
	glTranslatef(100,-50,45);
	glRotatef(270,0,0,1);
	
	glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, domimg);
	glBegin(GL_QUADS);{
		glTexCoord2f(0,1);
		glVertex3f(0,0,15);
		glTexCoord2f(0,0);
		glVertex3f(-50,0,15);
		glTexCoord2f(1,0);
		glVertex3f(-50,0,0);
		glTexCoord2f(1,1);
		glVertex3f(0,0,0);
	}glEnd();
  }glDisable(GL_TEXTURE_2D);
}glPopMatrix();

// varanda front left side
	glPushMatrix();{
	//glColor3f(1,.2,.8);
	glColor3f(1,1,1);
	glTranslatef(-100,-50,45);
	glRotatef(270,0,0,1);
	
	glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, domimg);
	glBegin(GL_QUADS);{
		glTexCoord2f(0,1);
		glVertex3f(0,0,15);
		glTexCoord2f(0,0);
		glVertex3f(-50,0,15);
		glTexCoord2f(1,0);
		glVertex3f(-50,0,0);
		glTexCoord2f(1,1);
		glVertex3f(0,0,0);
	}glEnd();
  }glDisable(GL_TEXTURE_2D);
	}glPopMatrix();


//varanda floor////

	
	glPushMatrix();{
	glColor3f(1,1,1);
	glTranslatef(-100,-47,0.5);
	glRotatef(270,1,0,0);
		glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, vfloorimg);
	glBegin(GL_QUADS);{
		
	   glTexCoord2f(0,1);
		glVertex3f(0,0,47.5);
		
	   glTexCoord2f(0,0);
		glVertex3f(200,0,47.5);
		
	   glTexCoord2f(1,0);
		glVertex3f(200,0,0);
	
	   glTexCoord2f(1,1);
		glVertex3f(0,0,0);
	}glEnd();
	
		
   }glDisable(GL_TEXTURE_2D);	
 
}glPopMatrix();


////////////end of varanda///////////////

///// CLIPPING OF SPH 01 middle dom

	glPushMatrix();{

		// up dom1		
		glPushMatrix();{
		glColor3f(.1,.1,.1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=1;
		glTranslatef(0,-10,87.8);
		//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,3,25,25);
			}glDisable(GL_CLIP_PLANE0);
	   // }glDisable(GL_TEXTURE_2D);
	}glPopMatrix();
		// up dom2		
		glPushMatrix();{

			//  up  cylinder2
		glPushMatrix();{
			glTranslatef(0,-10,92);
			glColor3f(.1,.1,.1);
			//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
		//GLUquadricObj *obj = gluNewQuadric();
		gluCylinder(obj,2,0.001,6,30,30);
		  //}glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();
		glColor3f(1,1,1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=1;
		glTranslatef(0,-10,91);
		//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,3,25,25);
			}glDisable(GL_CLIP_PLANE0);
	    //}glDisable(GL_TEXTURE_2D);
	}glPopMatrix();


		//  up  cylinder
		glPushMatrix();{
			glTranslatef(0,-10,79);
			glColor3f(1,1,1);
			glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
		gluCylinder(obj,8,3,8,25,25);
		  }glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();
		glColor3f(1,1,1);
		
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=1;
		glTranslatef(0,-10,61);
		glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, gumbojimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,22,25,25);
			}glDisable(GL_CLIP_PLANE0);
	    }glDisable(GL_TEXTURE_2D);
	}glPopMatrix();

	//02right dom

	//up dom1		
		glPushMatrix();{
		glColor3f(.1,.1,.1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=1;
		glTranslatef(47,-12,94.2);
		//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,3,25,25);
			}glDisable(GL_CLIP_PLANE0);
	    //}glDisable(GL_TEXTURE_2D);
	}glPopMatrix();
		// up dom2		
		glPushMatrix();{

			//  up  cylinder2
		glPushMatrix();{
			glTranslatef(47,-12,99.2);
			glColor3f(.1,.1,.1);
			//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
		//GLUquadricObj *obj = gluNewQuadric();
		gluCylinder(obj,2,0.001,6,30,30);
		  //}glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();
		glColor3f(1,1,1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=1;
		glTranslatef(47,-12,97.3);
		//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,3,25,25);
			}glDisable(GL_CLIP_PLANE0);
	    //}glDisable(GL_TEXTURE_2D);
	}glPopMatrix();

		glPushMatrix();{

			//  up  cylinder
		glPushMatrix();{
			glTranslatef(47,-12,85.6);
			glColor3f(1,1,1);
			glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
		gluCylinder(obj,8,3,8,25,25);
		  }glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();
		glColor3f(1,1,1);
		///// CLIPPING OF SPHERE

		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=1;
		glTranslatef(47,-12,61);
		glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, gumbojimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,26,30,30);
			}glDisable(GL_CLIP_PLANE0);
	    }glDisable(GL_TEXTURE_2D);
	}glPopMatrix();

	//03 left dom

	// up dom1		
		glPushMatrix();{
	glColor3f(.1,.1,.1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=1;
		glTranslatef(-47,-12,94.2);
		//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,3,25,25);
			}glDisable(GL_CLIP_PLANE0);
	    //}glDisable(GL_TEXTURE_2D);
	}glPopMatrix();
		// up dom2		
		glPushMatrix();{

			//  up  cylinder2
		glPushMatrix();{
			glTranslatef(-47,-12,99.2);
			glColor3f(.1,.1,.1);
			//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
		//GLUquadricObj *obj = gluNewQuadric();
		gluCylinder(obj,2,0.001,6,30,30);
		  //}glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();
		glColor3f(1,1,1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=1;
		glTranslatef(-47,-12,97.3);
		//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,3,25,25);
			}glDisable(GL_CLIP_PLANE0);
	    //}glDisable(GL_TEXTURE_2D);
	}glPopMatrix();

		glPushMatrix();{

			//  up  cylinder
		glPushMatrix();{
			glTranslatef(-47,-12,85.6);
			glColor3f(1,1,1);
			glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
		gluCylinder(obj,8,3,8,25,25);
		  }glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();
		glColor3f(1,1,1);
		///// CLIPPING OF SPHERE

		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=1;
		glTranslatef(-47,-12,61);
		glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, gumbojimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,26,30,30);
			}glDisable(GL_CLIP_PLANE0);
	    }glDisable(GL_TEXTURE_2D);
	}glPopMatrix();

	
//04 left side1 dom
		
		

		// up dom1		
		glPushMatrix();{
		glColor3f(.1,.1,.1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=2;
		glTranslatef(-84,-18,81.6);
		//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,3,25,25);
			}glDisable(GL_CLIP_PLANE0);
	    //}glDisable(GL_TEXTURE_2D);
	}glPopMatrix();
		// up dom2		
		glPushMatrix();{

			//  up  cylinder2
		glPushMatrix();{
			glTranslatef(-84,-18,87.6);
			glColor3f(.1,.1,.1);
			//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
		//GLUquadricObj *obj = gluNewQuadric();
		gluCylinder(obj,2,0.001,6,30,30);
		  //}glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();
		glColor3f(1,1,1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=2;
		glTranslatef(-84,-18,85.3);
		//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
	   //		gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,3,25,25);
			}glDisable(GL_CLIP_PLANE0);
	    //}glDisable(GL_TEXTURE_2D);
	}glPopMatrix();
		
		glPushMatrix();{

			//  up  cylinder
		glPushMatrix();{
			glTranslatef(-84,-18,73);
			glColor3f(1,1,1);
			glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
		gluCylinder(obj,6,3,8,25,25);
		  }glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();
		glColor3f(1,1,1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=1;
		glTranslatef(-84,-18,61);
		glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, gumbojimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,15,25,25);
			}glDisable(GL_CLIP_PLANE0);
	    }glDisable(GL_TEXTURE_2D);
	}glPopMatrix();

//04 left back2 dom
		// up dom1		
		glPushMatrix();{
		glColor3f(.1,.1,.1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=2;
		glTranslatef(-68,32,88.3);
		//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,3,25,25);
			}glDisable(GL_CLIP_PLANE0);
	    //}glDisable(GL_TEXTURE_2D);
	}glPopMatrix();
		// up dom2		
		glPushMatrix();{

			//  up  cylinder2
		glPushMatrix();{
			glTranslatef(-68,32,94);
			glColor3f(.1,.1,.1);
			//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
		//GLUquadricObj *obj = gluNewQuadric();
		gluCylinder(obj,2,0.001,6,30,39);
		  //}glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();
		glColor3f(1,1,1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=2;
		glTranslatef(-68,32,92);
		//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,3,25,25);
			}glDisable(GL_CLIP_PLANE0);
	    //}glDisable(GL_TEXTURE_2D);
	}glPopMatrix();
		glPushMatrix();{
				//  up  cylinder
		glPushMatrix();{
			glTranslatef(-68,32,80);
			glColor3f(1,1,1);
			glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
		gluCylinder(obj,8,3,8,25,25);
		  }glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();

		glColor3f(1,1,1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=1;
		glTranslatef(-68,32,61);
		glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, gumbojimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,20,30,30);
			}glDisable(GL_CLIP_PLANE0);
	    }glDisable(GL_TEXTURE_2D);
	}glPopMatrix();
// left back3 dom


		// up dom1		
		glPushMatrix();{
		glColor3f(.1,.1,.1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=2;
		glTranslatef(-68,75,87.8);
		//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,3,25,25);
			}glDisable(GL_CLIP_PLANE0);
	    //}glDisable(GL_TEXTURE_2D);
	}glPopMatrix();
		// up dom2		
		glPushMatrix();{

			//  up  cylinder2
		glPushMatrix();{
			glTranslatef(-68,75,94);
			glColor3f(.1,.1,.1);
			//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
		gluCylinder(obj,2,0.001,6,30,30);
		  //}glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();
		glColor3f(1,1,1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=2;
		glTranslatef(-68,75,92);
		//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,3,25,25);
			}glDisable(GL_CLIP_PLANE0);
	    //}glDisable(GL_TEXTURE_2D);
	}glPopMatrix();
		
		
		glPushMatrix();{
			//  up  cylinder
		glPushMatrix();{
			glTranslatef(-68,75,79);
			glColor3f(1,1,1);
			glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
		gluCylinder(obj,8,3,8,25,25);
		  }glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();
		glColor3f(1,1,1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=1;
		glTranslatef(-68,75,60);
		glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, gumbojimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,21,25,25);
			}glDisable(GL_CLIP_PLANE0);
	    }glDisable(GL_TEXTURE_2D);
	}glPopMatrix();

//right side1 dom
		
		// up dom1		
		glPushMatrix();{
		glColor3f(.1,.1,.1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=2;
		glTranslatef(84,-18,81.6);
		//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,3,25,25);
			}glDisable(GL_CLIP_PLANE0);
	    //}glDisable(GL_TEXTURE_2D);
	}glPopMatrix();
		// up dom2		
		glPushMatrix();{

			//  up  cylinder2
		glPushMatrix();{
			glTranslatef(84,-18,87.6);
			glColor3f(.1,.1,.1);
			//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
		//GLUquadricObj *obj = gluNewQuadric();
		gluCylinder(obj,2,0.001,6,30,30);
		  //}glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();
		glColor3f(1,1,1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=2;
		glTranslatef(84,-18,85.3);
		//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
	   //		gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,3,25,25);
			}glDisable(GL_CLIP_PLANE0);
	    //}glDisable(GL_TEXTURE_2D);
	}glPopMatrix();
		
		glPushMatrix();{

			//  up  cylinder
		glPushMatrix();{
			glTranslatef(84,-18,73);
			glColor3f(1,1,1);
			glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
		gluCylinder(obj,6,3,8,25,25);
		  }glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();
		glColor3f(1,1,1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=1;
		glTranslatef(84,-18,61);
		glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, gumbojimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,15,25,25);
			}glDisable(GL_CLIP_PLANE0);
	    }glDisable(GL_TEXTURE_2D);
	}glPopMatrix();



//04 right back2 dom
		// up dom1		
		glPushMatrix();{
		glColor3f(.1,.1,.1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=2;
		glTranslatef(68,32,88.3);
		//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,3,25,25);
			}glDisable(GL_CLIP_PLANE0);
	    //}glDisable(GL_TEXTURE_2D);
	}glPopMatrix();
		// up dom2		
		glPushMatrix();{

			//  up  cylinder2
		glPushMatrix();{
			glTranslatef(68,32,94);
			glColor3f(.1,.1,.1);
			//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
		//GLUquadricObj *obj = gluNewQuadric();
		gluCylinder(obj,2,0.001,6,30,39);
		  //}glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();
		glColor3f(1,1,1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=2;
		glTranslatef(68,32,92);
		//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,3,25,25);
			}glDisable(GL_CLIP_PLANE0);
	    //}glDisable(GL_TEXTURE_2D);
	}glPopMatrix();
		glPushMatrix();{
				//  up  cylinder
		glPushMatrix();{
			glTranslatef(68,32,80);
			glColor3f(1,1,1);
			glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
		gluCylinder(obj,8,3,8,25,25);
		  }glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();

		glColor3f(1,1,1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=1;
		glTranslatef(68,32,61);
		glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, gumbojimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,20,30,30);
			}glDisable(GL_CLIP_PLANE0);
	    }glDisable(GL_TEXTURE_2D);
	}glPopMatrix();


// right back3 dom

	// up dom1		
		glPushMatrix();{
		glColor3f(.1,.1,.1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=2;
		glTranslatef(68,75,87.8);
		//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,3,25,25);
			}glDisable(GL_CLIP_PLANE0);
	    //}glDisable(GL_TEXTURE_2D);
	}glPopMatrix();
		// up dom2		
		glPushMatrix();{

			//  up  cylinder2
		glPushMatrix();{
			glTranslatef(68,75,94);
			glColor3f(.1,.1,.1);
			//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
		gluCylinder(obj,2,0.001,6,30,30);
		  //}glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();
		glColor3f(1,1,1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=2;
		glTranslatef(68,75,92);
		//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,3,25,25);
			}glDisable(GL_CLIP_PLANE0);
	    //}glDisable(GL_TEXTURE_2D);
	}glPopMatrix();
		
		
		glPushMatrix();{
			//  up  cylinder
		glPushMatrix();{
			glTranslatef(68,75,79);
			glColor3f(1,1,1);
			glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
		gluCylinder(obj,8,3,8,25,25);
		  }glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();
		glColor3f(1,1,1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=1;
		glTranslatef(68,75,60);
		glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, gumbojimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,21,25,25);
			}glDisable(GL_CLIP_PLANE0);
	    }glDisable(GL_TEXTURE_2D);
	}glPopMatrix();

/////  01 back middle dom

	glPushMatrix();{

		// up dom1		
		glPushMatrix();{
		glColor3f(.1,.1,.1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=1;
		glTranslatef(0,65,87.8);
		//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,3,25,25);
			}glDisable(GL_CLIP_PLANE0);
	   // }glDisable(GL_TEXTURE_2D);
	}glPopMatrix();
		// up dom2		
		glPushMatrix();{

			//  up  cylinder2
		glPushMatrix();{
			glTranslatef(0,65,92);
			glColor3f(.1,.1,.1);
			//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
		//GLUquadricObj *obj = gluNewQuadric();
		gluCylinder(obj,2,0.001,6,30,30);
		  //}glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();
		glColor3f(1,1,1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=1;
		glTranslatef(0,65,91);
		//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,3,25,25);
			}glDisable(GL_CLIP_PLANE0);
	    //}glDisable(GL_TEXTURE_2D);
	}glPopMatrix();


		//  up  cylinder
		glPushMatrix();{
			glTranslatef(0,65,79);
			glColor3f(1,1,1);
			glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
		gluCylinder(obj,8,3,8,25,25);
		  }glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();
		glColor3f(1,1,1);
		
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=1;
		glTranslatef(0,65,61);
		glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, gumbojimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,22,25,25);
			}glDisable(GL_CLIP_PLANE0);
	    }glDisable(GL_TEXTURE_2D);
	}glPopMatrix();

////// start of big cylinders//////////////////

// cylinder front right
		glPushMatrix();{
			// up dom2		
		glPushMatrix();{

			//  up  cylinder2
		glPushMatrix();{
			glTranslatef(103,-52.5,78);
			glColor3f(.1,.1,.1);
			//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
		//GLUquadricObj *obj = gluNewQuadric();
		gluCylinder(obj,3,0.1,8,30,30);
		  //}glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();
		glColor3f(1,1,1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=15;
		glTranslatef(103,-52.5,75);
		glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, gumbojimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,6,25,25);
			}glDisable(GL_CLIP_PLANE0);
	    }glDisable(GL_TEXTURE_2D);
	}glPopMatrix();
			glPushMatrix();{
			glColor3f(.1,.1,.1);
	glTranslatef(103,-52.5,72);
	glScalef(12,12,2);
	glutSolidCube(1);
	}glPopMatrix();
			// cube and cyl1
   glPushMatrix();{
	glPushMatrix();{
		
				glTranslatef(103,-52.5,3);
			GLUquadricObj *obj = gluNewQuadric();
			glColor3f(.1,.1,.1);
			gluCylinder(obj,6,6,5,15,15);
			}glPopMatrix();
	glColor3f(.1,.1,.1);
	glTranslatef(103,-52.5,2);
	glScalef(12,12,4);
	glutSolidCube(1);
	}glPopMatrix();
			glTranslatef(103,-52.5,8);
			glColor3f(1,1,1);
			glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
		gluCylinder(obj,6,6,65,25,25);
		  }glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();


// cylinder front left 
		glPushMatrix();{
			// up dom2		
		glPushMatrix();{

			//  up  cylinder2
		glPushMatrix();{
			glTranslatef(-103,-52.5,78);
			glColor3f(.1,.1,.1);
			//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
		//GLUquadricObj *obj = gluNewQuadric();
		gluCylinder(obj,3,0.1,8,30,30);
		  //}glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();
		glColor3f(1,1,1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=15;
		glTranslatef(-103,-52.5,75);
		glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, gumbojimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,6,25,25);
			}glDisable(GL_CLIP_PLANE0);
	    }glDisable(GL_TEXTURE_2D);
	}glPopMatrix();
			glPushMatrix();{
			glColor3f(.1,.1,.1);
	glTranslatef(-103,-52.5,72);
	glScalef(12,12,2);
	glutSolidCube(1);
	}glPopMatrix();
			// cube and cyl1
   glPushMatrix();{
	glPushMatrix();{
		
				glTranslatef(-103,-52.5,3);
			GLUquadricObj *obj = gluNewQuadric();
			glColor3f(.1,.1,.1);
			gluCylinder(obj,6,6,5,15,15);
			}glPopMatrix();
	glColor3f(.1,.1,.1);
	glTranslatef(-103,-52.5,2);
	glScalef(12,12,4);
	glutSolidCube(1);
	}glPopMatrix();
			glTranslatef(-103,-52.5,8);
			glColor3f(1,1,1);
			glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
		gluCylinder(obj,6,6,65,25,25);
		  }glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();


// cylinder back left 
		glPushMatrix();{
			// up dom2		
		glPushMatrix();{

			//  up  cylinder2
		glPushMatrix();{
			glTranslatef(-103,104.3,78);
			glColor3f(.1,.1,.1);
			//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
		//GLUquadricObj *obj = gluNewQuadric();
		gluCylinder(obj,3,0.1,8,30,30);
		  //}glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();
		glColor3f(1,1,1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=15;
		glTranslatef(-103,104.3,75);
		glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, gumbojimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,6,25,25);
			}glDisable(GL_CLIP_PLANE0);
	    }glDisable(GL_TEXTURE_2D);
	}glPopMatrix();
			glPushMatrix();{
			glColor3f(.1,.1,.1);
	glTranslatef(-103,104.3,72);
	glScalef(12,12,2);
	glutSolidCube(1);
	}glPopMatrix();
			// cube and cyl1
   glPushMatrix();{
	glPushMatrix();{
		
				glTranslatef(-103,104.3,3);
			GLUquadricObj *obj = gluNewQuadric();
			glColor3f(.1,.1,.1);
			gluCylinder(obj,6,6,5,15,15);
			}glPopMatrix();
	glColor3f(.1,.1,.1);
	glTranslatef(-103,104.3,2);
	glScalef(12,12,4);
	glutSolidCube(1);
	}glPopMatrix();
			glTranslatef(-103,104.3,8);
			glColor3f(1,1,1);
			glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
		
		gluCylinder(obj,6,6,65,25,25);
		  }glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();




// cylinder back right 
		glPushMatrix();{
			// up dom2		
		glPushMatrix();{

			//  up  cylinder2
		glPushMatrix();{
			glTranslatef(103,104.3,78);
			glColor3f(.1,.1,.1);
			//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
		//GLUquadricObj *obj = gluNewQuadric();
		gluCylinder(obj,3,0.1,8,30,30);
		  //}glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();
		glColor3f(1,1,1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=15;
		glTranslatef(103,104.3,75);
		glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, gumbojimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,6,25,25);
			}glDisable(GL_CLIP_PLANE0);
	    }glDisable(GL_TEXTURE_2D);
	}glPopMatrix();
			glPushMatrix();{
			glColor3f(.1,.1,.1);
	glTranslatef(103,104.3,72);
	glScalef(12,12,2);
	glutSolidCube(1);
	}glPopMatrix();
			// cube and cyl1
   glPushMatrix();{
	glPushMatrix();{
		
				glTranslatef(103,104.3,3);
			GLUquadricObj *obj = gluNewQuadric();
			glColor3f(.1,.1,.1);
			gluCylinder(obj,6,6,5,15,15);
			}glPopMatrix();
	glColor3f(.1,.1,.1);
	glTranslatef(103,104.3,2);
	glScalef(12,12,4);
	glutSolidCube(1);
	}glPopMatrix();
			glTranslatef(103,104.3,8);
			glColor3f(1,1,1);
			glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
		
		gluCylinder(obj,6,6,65,25,25);
		  }glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix(); 

///////////     roof cylinders

// 01
		glPushMatrix();{
			// up dom2		
		glPushMatrix();{

			//  up  cylinder2
		glPushMatrix();{
			glTranslatef(94,-40,74);
			glColor3f(.1,.1,.1);
			//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
		//GLUquadricObj *obj = gluNewQuadric();
		gluCylinder(obj,1,0.1,4,30,30);
		  //}glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();
		glColor3f(1,1,1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=15;
		glTranslatef(94,-40,73);
		//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,2,25,25);
			}glDisable(GL_CLIP_PLANE0);
	    //}glDisable(GL_TEXTURE_2D);
	}glPopMatrix();
			glPushMatrix();{
			glColor3f(.1,.1,.1);
	glTranslatef(94,-40,72);
	glScalef(4,4,1);
	glutSolidCube(1);
	}glPopMatrix();
			
			glTranslatef(94,-40,60);
			glColor3f(1,1,1);
			glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
		
		gluCylinder(obj,2,2,12,25,25);
		  }glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();


///02 

	glPushMatrix();{
			// up dom2		
		glPushMatrix();{

			//  up  cylinder2
		glPushMatrix();{
			glTranslatef(74,-40,74);
			glColor3f(.1,.1,.1);
			//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
		//GLUquadricObj *obj = gluNewQuadric();
		gluCylinder(obj,1,0.1,4,30,30);
		  //}glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();
		glColor3f(1,1,1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=15;
		glTranslatef(74,-40,73);
		//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,2,25,25);
			}glDisable(GL_CLIP_PLANE0);
	    //}glDisable(GL_TEXTURE_2D);
	}glPopMatrix();
			glPushMatrix();{
			glColor3f(.1,.1,.1);
	glTranslatef(74,-40,72);
	glScalef(4,4,1);
	glutSolidCube(1);
	}glPopMatrix();
			
			glTranslatef(74,-40,60);
			glColor3f(1,1,1);
			glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
	
		gluCylinder(obj,2,2,12,25,25);
		  }glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();


///03

	glPushMatrix();{
			// up dom2		
		glPushMatrix();{

			//  up  cylinder2
		glPushMatrix();{
			glTranslatef(49,-40,74);
			glColor3f(.1,.1,.1);
			//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
		//GLUquadricObj *obj = gluNewQuadric();
		gluCylinder(obj,1,0.1,4,30,30);
		  //}glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();
		glColor3f(1,1,1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=15;
		glTranslatef(49,-40,73);
		//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,2,25,25);
			}glDisable(GL_CLIP_PLANE0);
	    //}glDisable(GL_TEXTURE_2D);
	}glPopMatrix();
			glPushMatrix();{
			glColor3f(.1,.1,.1);
	glTranslatef(49,-40,72);
	glScalef(4,4,1);
	glutSolidCube(1);
	}glPopMatrix();
			
			glTranslatef(49,-40,60);
			glColor3f(1,1,1);
			glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
		
		gluCylinder(obj,2,2,12,25,25);
		  }glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();


///04

	glPushMatrix();{
			// up dom2		
		glPushMatrix();{

			//  up  cylinder2
		glPushMatrix();{
			glTranslatef(24,-40,74);
			glColor3f(.1,.1,.1);
			//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
		//GLUquadricObj *obj = gluNewQuadric();
		gluCylinder(obj,1,0.1,4,30,30);
		  //}glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();
		glColor3f(1,1,1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=15;
		glTranslatef(24,-40,73);
		//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,2,25,25);
			}glDisable(GL_CLIP_PLANE0);
	    //}glDisable(GL_TEXTURE_2D);
	}glPopMatrix();
			glPushMatrix();{
			glColor3f(.1,.1,.1);
	glTranslatef(24,-40,72);
	glScalef(4,4,1);
	glutSolidCube(1);
	}glPopMatrix();
			
			glTranslatef(24,-40,60);
			glColor3f(1,1,1);
			glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
	
		gluCylinder(obj,2,2,12,25,25);
		  }glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();


///05

	glPushMatrix();{
			// up dom2		
		glPushMatrix();{

			//  up  cylinder2
		glPushMatrix();{
			glTranslatef(-2,-40,74);
			glColor3f(.1,.1,.1);
			//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
		//GLUquadricObj *obj = gluNewQuadric();
		gluCylinder(obj,1,0.1,4,30,30);
		  //}glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();
		glColor3f(1,1,1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=15;
		glTranslatef(-2,-40,73);
		//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,2,25,25);
			}glDisable(GL_CLIP_PLANE0);
	    //}glDisable(GL_TEXTURE_2D);
	}glPopMatrix();
			glPushMatrix();{
			glColor3f(.1,.1,.1);
	glTranslatef(-2,-40,72);
	glScalef(4,4,1);
	glutSolidCube(1);
	}glPopMatrix();
			
			glTranslatef(-2,-40,60);
			glColor3f(1,1,1);
			glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
	
		gluCylinder(obj,2,2,12,25,25);
		  }glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();



////06

	glPushMatrix();{
			// up dom2		
		glPushMatrix();{

			//  up  cylinder2
		glPushMatrix();{
			glTranslatef(-26,-40,74);
			glColor3f(.1,.1,.1);
			//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
		//GLUquadricObj *obj = gluNewQuadric();
		gluCylinder(obj,1,0.1,4,30,30);
		  //}glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();
		glColor3f(1,1,1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=15;
		glTranslatef(-26,-40,73);
		//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,2,25,25);
			}glDisable(GL_CLIP_PLANE0);
	    //}glDisable(GL_TEXTURE_2D);
	}glPopMatrix();
			glPushMatrix();{
			glColor3f(.1,.1,.1);
	glTranslatef(-26,-40,72);
	glScalef(4,4,1);
	glutSolidCube(1);
	}glPopMatrix();
			
			glTranslatef(-26,-40,60);
			glColor3f(1,1,1);
			glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
		
		gluCylinder(obj,2,2,12,25,25);
		  }glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();


///07

	glPushMatrix();{
			// up dom2		
		glPushMatrix();{

			//  up  cylinder2
		glPushMatrix();{
			glTranslatef(-51,-40,74);
			glColor3f(.1,.1,.1);
			//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
		//GLUquadricObj *obj = gluNewQuadric();
		gluCylinder(obj,1,0.1,4,30,30);
		  //}glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();
		glColor3f(1,1,1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=15;
		glTranslatef(-51,-40,73);
		//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,2,25,25);
			}glDisable(GL_CLIP_PLANE0);
	    //}glDisable(GL_TEXTURE_2D);
	}glPopMatrix();
			glPushMatrix();{
			glColor3f(.1,.1,.1);
	glTranslatef(-51,-40,72);
	glScalef(4,4,1);
	glutSolidCube(1);
	}glPopMatrix();
			
			glTranslatef(-51,-40,60);
			glColor3f(1,1,1);
			glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
		
		gluCylinder(obj,2,2,12,25,25);
		  }glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();


//08

	glPushMatrix();{
			// up dom2		
		glPushMatrix();{

			//  up  cylinder2
		glPushMatrix();{
			glTranslatef(-76,-40,74);
			glColor3f(.1,.1,.1);
			//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
		//GLUquadricObj *obj = gluNewQuadric();
		gluCylinder(obj,1,0.1,4,30,30);
		  //}glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();
		glColor3f(1,1,1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=15;
		glTranslatef(-76,-40,73);
		//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,2,25,25);
			}glDisable(GL_CLIP_PLANE0);
	    //}glDisable(GL_TEXTURE_2D);
	}glPopMatrix();
			glPushMatrix();{
			glColor3f(.1,.1,.1);
	glTranslatef(-76,-40,72);
	glScalef(4,4,1);
	glutSolidCube(1);
	}glPopMatrix();
			
			glTranslatef(-76,-40,60);
			glColor3f(1,1,1);
			glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
	
		gluCylinder(obj,2,2,12,25,25);
		  }glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();


//09

	glPushMatrix();{
			// up dom2		
		glPushMatrix();{

			//  up  cylinder2
		glPushMatrix();{
			glTranslatef(-97,-40,74);
			glColor3f(.1,.1,.1);
			//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
		//GLUquadricObj *obj = gluNewQuadric();
		gluCylinder(obj,1,0.1,4,30,30);
		  //}glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();
		glColor3f(1,1,1);
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=15;
		glTranslatef(-97,-40,73);
		//glEnable(GL_TEXTURE_2D);{
			//glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			//gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,2,25,25);
			}glDisable(GL_CLIP_PLANE0);
	    //}glDisable(GL_TEXTURE_2D);
	}glPopMatrix();
			glPushMatrix();{
			glColor3f(.1,.1,.1);
	glTranslatef(-97,-40,72);
	glScalef(4,4,1);
	glutSolidCube(1);
	}glPopMatrix();
			
			glTranslatef(-97,-40,60);
			glColor3f(1,1,1);
			glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
		
		gluCylinder(obj,2,2,12,25,25);
		  }glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();
////////   end of roof cylinder/////////

////***  start of ground
	////The ground


glPushMatrix();{
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, groundfimg);
	glBegin(GL_QUADS); {
		glTexCoord2f(0,1);
		glVertex3f(1000, -1000, 0);
		glTexCoord2f(0,0);
		glVertex3f(-1000, -1000, 0);
		glTexCoord2f(1,0);
		glVertex3f(-1000, 1000, 0);
		glTexCoord2f(1,1);
		glVertex3f(1000, 1000, 0);
	}
	glEnd();

   }glDisable(GL_TEXTURE_2D);
}glPopMatrix();

//ground left side

	
	glPushMatrix();{
	glColor3f(1,1,1);
	glTranslatef(-50,-100,2);
	glRotatef(270,1,0,0);
		glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, grassimg);
	glBegin(GL_QUADS);{
		
	   glTexCoord2f(0,1);
		glVertex3f(0,0,30);
		
	   glTexCoord2f(0,0);
		glVertex3f(-50,0,30);
		
	   glTexCoord2f(1,0);
		glVertex3f(-50,0,0);
	
	   glTexCoord2f(1,1);
		glVertex3f(0,0,0);
	}glEnd();
	
		
   }glDisable(GL_TEXTURE_2D);	
 
}glPopMatrix();
	
	//ground right side

	
	glPushMatrix();{
	glColor3f(1,1,1);
	glTranslatef(50,-100,2);
	glRotatef(270,1,0,0);
		glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, grassimg);
	glBegin(GL_QUADS);{
		
	   glTexCoord2f(0,1);
		glVertex3f(0,0,30);
		
	   glTexCoord2f(0,0);
		glVertex3f(50,0,30);
		
	   glTexCoord2f(1,0);
		glVertex3f(50,0,0);
	
	   glTexCoord2f(1,1);
		glVertex3f(0,0,0);
	}glEnd();
	
		
   }glDisable(GL_TEXTURE_2D);	
 
}glPopMatrix();



	//ground middle 

	
	glPushMatrix();{
	glColor3f(1,1,1);
	glTranslatef(-25,-100,2);
	glRotatef(270,1,0,0);
		glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, grassimg);
	glBegin(GL_QUADS);{
		
	   glTexCoord2f(0,1);
		glVertex3f(0,0,30);
		
	   glTexCoord2f(0,0);
		glVertex3f(50,0,30);
		
	   glTexCoord2f(1,0);
		glVertex3f(50,0,0);
	
	   glTexCoord2f(1,1);
		glVertex3f(0,0,0);
	}glEnd();
	
		
   }glDisable(GL_TEXTURE_2D);	
 
}glPopMatrix();


//ground too front 

	
	/*glPushMatrix();{
	glColor3f(1,1,1);
	glTranslatef(-70,-150,2);
	glRotatef(270,1,0,0);
		glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, taraimg);
	glBegin(GL_QUADS);{
		
	   glTexCoord2f(0,1);
		glVertex3f(0,0,50);
		
	   glTexCoord2f(0,0);
		glVertex3f(140,0,50);
		
	   glTexCoord2f(1,0);
		glVertex3f(140,0,0);
	
	   glTexCoord2f(1,1);
		glVertex3f(0,0,0);
	}glEnd();
	
		
   }glDisable(GL_TEXTURE_2D);	
 
}glPopMatrix(); */

///////////// end of ground/////////////

	
	//sky

	glPushMatrix();{
	glColor3f(1,1,1);
		///// CLIPPING OF SPHERE

		
		glTranslatef(0,0,0);

		glPushMatrix();{
		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=0;
		glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				

			
            glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, skyimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
			
				gluSphere(obj,1000,300,300);
			
	    }
		glDisable(GL_TEXTURE_2D);
	}
			}glDisable(GL_CLIP_PLANE0);
}glPopMatrix();
			
			glPopMatrix();


// right pray cylinder

	// cylinder front right
		glPushMatrix();{
			glTranslatef(102,52,0);
			glColor3f(1,1,1);
			glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
		gluCylinder(obj,15,15,30,30,30);
		  }glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();
//pray dom
		glPushMatrix();{
glColor3f(1,1,1);
		///// CLIPPING OF SPHERE

		double equ[4];
		equ[0]=0;
		equ[1]=0;
		equ[2]=1;
		equ[3]=0.3;
		glTranslatef(102,52,30.3);
		glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, gumbojimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
			glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				gluSphere(obj,15,25,25);
			}glDisable(GL_CLIP_PLANE0);
	    }glDisable(GL_TEXTURE_2D);
	}glPopMatrix(); 
///// end of big cyl//////////////

///// start of front floor cyl../////

//front wall door  cylinder1
		
		glPushMatrix();{
			glTranslatef(90,-52.5,7);
			glColor3f(1,1,1);
			glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
		
		gluCylinder(obj,3,3,25,15,15);
		  }glDisable(GL_TEXTURE_2D);	//
				
}glPopMatrix();

//front wall door  cylinder2
		glPushMatrix();{
			glTranslatef(65,-52.5,7);
			glColor3f(1,1,1);
			glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
		
		gluCylinder(obj,3,3,25,15,15);
		  }glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();

//front wall door  cylinder3
		glPushMatrix();{
			glTranslatef(40,-52.5,7);
			glColor3f(1,1,1);
			glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
		
		gluCylinder(obj,3,3,25,15,15);
		  }glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();

//front wall door  cylinder4
		glPushMatrix();{
			glTranslatef(15,-52.5,7);
			glColor3f(1,1,1);
			glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
		gluCylinder(obj,3,3,25,15,15);
		  }glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();

//front wall door  cylinder5
		glPushMatrix();{
			glTranslatef(-15,-52.5,7);
			glColor3f(1,1,1);
			glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
	
		gluCylinder(obj,3,3,25,15,15);
		  }glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();

//front wall door cylinder6
		glPushMatrix();{
			glTranslatef(-40,-52.5,7);
			glColor3f(1,1,1);
			glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
		
		gluCylinder(obj,3,3,25,15,15);
		 }glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();

//front wall door cylinder7
		glPushMatrix();{
			glTranslatef(-65,-52.5,7);
			glColor3f(1,1,1);
			glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
		
		gluCylinder(obj,3,3,25,15,15);
		  }glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();

//front wall door cylinder8
		glPushMatrix();{
			glTranslatef(-90,-52.5,7);
			glColor3f(1,1,1);
			glEnable(GL_TEXTURE_2D);{
			glBindTexture(GL_TEXTURE_2D, cylimg);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj,GL_TRUE);
	
		gluCylinder(obj,3,3,25,15,15);
		  }glDisable(GL_TEXTURE_2D);	
				
}glPopMatrix();

////////////  end of front floor cyl/////////

///// start of door curve/////////


// door curve1
glPushMatrix();{
	double equ[4];
		equ[0]=0;
		equ[1]=1;
		equ[2]=0;
		equ[3]=0;
		glTranslatef(77.5,-51,37);
		glRotatef(-270,1,0,0);
	glColor3f(1,1,1);
	glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				glutSolidTorus(2,15,25,25);

			}glDisable(GL_CLIP_PLANE0);
}glPopMatrix();
// door curve2
glPushMatrix();{
	double equ[4];
		equ[0]=0;
		equ[1]=1;
		equ[2]=0;
		equ[3]=0;
		glTranslatef(52.3,-51,37);
		glRotatef(-270,1,0,0);
	glColor3f(1,1,1);
	glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				glutSolidTorus(2,15,25,25);

			}glDisable(GL_CLIP_PLANE0);
}glPopMatrix();

// door curve3
glPushMatrix();{
	double equ[4];
		equ[0]=0;
		equ[1]=1;
		equ[2]=0;
		equ[3]=0;
		glTranslatef(27,-51,37);
		glRotatef(-270,1,0,0);
	glColor3f(1,1,1);
	glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				glutSolidTorus(2,15,25,25);

			}glDisable(GL_CLIP_PLANE0);
}glPopMatrix();


// door curve4
glPushMatrix();{
	double equ[4];
		equ[0]=0;
		equ[1]=1;
		equ[2]=0;
		equ[3]=0;
		glTranslatef(0.3,-51,37);
		glRotatef(-270,1,0,0);
	glColor3f(1,1,1);
	glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				glutSolidTorus(2,16.5,25,25);

			}glDisable(GL_CLIP_PLANE0);
}glPopMatrix();

// door curve5
glPushMatrix();{
	double equ[4];
		equ[0]=0;
		equ[1]=1;
		equ[2]=0;
		equ[3]=0;
		glTranslatef(-26.5,-51,37);
		glRotatef(-270,1,0,0);
	   glColor3f(1,1,1);
	glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				glutSolidTorus(2,15,25,25);

			}glDisable(GL_CLIP_PLANE0);
}glPopMatrix();


// door curve6
glPushMatrix();{
	double equ[4];
		equ[0]=0;
		equ[1]=1;
		equ[2]=0;
		equ[3]=0;
		glTranslatef(-52,-51,37);
		glRotatef(-270,1,0,0);
	glColor3f(1,1,1);
	glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				glutSolidTorus(2,15,25,25);

			}glDisable(GL_CLIP_PLANE0);
}glPopMatrix();



// door curve7
glPushMatrix();{
	double equ[4];
		equ[0]=0;
		equ[1]=1;
		equ[2]=0;
		equ[3]=0;
		glTranslatef(-77.5,-51,37);
		glRotatef(-270,1,0,0);
       glColor3f(1,1,1);
	glClipPlane(GL_CLIP_PLANE0,equ);
			glEnable(GL_CLIP_PLANE0);{
				glutSolidTorus(2,15,25,25);

			}glDisable(GL_CLIP_PLANE0);
}glPopMatrix();

// cube and cyl8
glPushMatrix();{
	
	glPushMatrix();{
		
				glTranslatef(-90,-52.5,3);
			GLUquadricObj *obj = gluNewQuadric();
			glColor3f(1.0,1.0,1.0);
			gluCylinder(obj,4,3,4,15,15);
			}glPopMatrix();
	glColor3f(0.1,0.1,0.0);
	glTranslatef(-90,-52,2);
	glScalef(12,12,4);
	glutSolidCube(1);
	}glPopMatrix();
// cube and cyl7
glPushMatrix();{
	
	glPushMatrix();{
		
				glTranslatef(-65,-52.5,3);
			GLUquadricObj *obj = gluNewQuadric();
			glColor3f(1.0,1.0,1.0);
			gluCylinder(obj,4,3,4,15,15);
			}glPopMatrix();
	glColor3f(0.1,0.1,0.0);
	glTranslatef(-65,-52,2);
	glScalef(12,12,4);
	glutSolidCube(1);
	}glPopMatrix();

// cube and cyl6
glPushMatrix();{
	
	glPushMatrix();{
		
				glTranslatef(-40,-52.5,3);
			GLUquadricObj *obj = gluNewQuadric();
		glColor3f(1.0,1.0,1.0);
			gluCylinder(obj,4,3,4,15,15);
			}glPopMatrix();
	glColor3f(0.1,0.1,0.0);
	glTranslatef(-40,-52,2);
	glScalef(12,12,4);
	glutSolidCube(1);
	}glPopMatrix();

// cube and cyl5
glPushMatrix();{
	
	glPushMatrix();{
		
				glTranslatef(-15,-52.5,3);
			GLUquadricObj *obj = gluNewQuadric();
			glColor3f(1.0,1.0,1.0);
			gluCylinder(obj,4,3,4,15,15);
			}glPopMatrix();
	glColor3f(0.1,0.1,0.0);
	glTranslatef(-15,-52,2);
	glScalef(12,12,4);
	glutSolidCube(1);
	}glPopMatrix();

// cube and cyl4
glPushMatrix();{
	
	glPushMatrix();{
		
				glTranslatef(15,-52.5,3);
			GLUquadricObj *obj = gluNewQuadric();
	        glColor3f(1.0,1.0,1.0);	
			gluCylinder(obj,4,3,4,15,15);
			}glPopMatrix();
	glColor3f(0.1,0.1,0.0);
	glTranslatef(15,-52,2);
	glScalef(12,12,4);
	glutSolidCube(1);
	}glPopMatrix();

// cube and cyl3
glPushMatrix();{
	
	glPushMatrix();{
		
				glTranslatef(40,-52.5,3);
			GLUquadricObj *obj = gluNewQuadric();
			glColor3f(1.0,1.0,1.0);
			gluCylinder(obj,4,3,4,15,15);
			}glPopMatrix();
	glColor3f(0.1,0.1,0.0);
	glTranslatef(40,-52,2);
	glScalef(12,12,4);
	glutSolidCube(1);
	}glPopMatrix();

// cube and cyl2
glPushMatrix();{
	
	glPushMatrix();{
		
				glTranslatef(65,-52.5,3);
			GLUquadricObj *obj = gluNewQuadric();
			glColor3f(1.0,1.0,1.0);
			gluCylinder(obj,4,3,4,15,15);
			}glPopMatrix();
	glColor3f(0.1,0.1,0.0);
	glTranslatef(65,-52,2);
	glScalef(12,12,4);
	glutSolidCube(1);
	}glPopMatrix();


// cube and cyl1
glPushMatrix();{
	
	glPushMatrix();{
		
				glTranslatef(90,-52.5,3);
			GLUquadricObj *obj = gluNewQuadric();
			glColor3f(1.0,1.0,1.0);
			gluCylinder(obj,4,3,4,15,15);
			}glPopMatrix();
	glColor3f(0.1,0.1,0.0);
	glTranslatef(90,-52,2);
	glScalef(12,12,4);
	glutSolidCube(1);
	}glPopMatrix();

//up cube and cyl1
glPushMatrix();{
	
	glPushMatrix();{
		
				glTranslatef(90,-52.5,32);
			GLUquadricObj *obj = gluNewQuadric();
			glColor3f(1.0,1.0,1.0);
			gluCylinder(obj,3,3.5,4,15,15);
			}glPopMatrix();
	glColor3f(0.1,0.1,0.0);
	glTranslatef(90,-52,36);
	glScalef(8,8,2);
	glutSolidCube(1);
	}glPopMatrix();


//up cube and cyl8
glPushMatrix();{
	
	glPushMatrix();{
		
				glTranslatef(-90,-52.5,32);
			GLUquadricObj *obj = gluNewQuadric();
			glColor3f(1.0,1.0,1.0);
			gluCylinder(obj,3,3.5,4,15,15);
			}glPopMatrix();
	glColor3f(0.1,0.1,0.0);
	glTranslatef(-90,-52,36);
	glScalef(8,8,2);
	glutSolidCube(1);
	}glPopMatrix();
// up cube and cyl7
glPushMatrix();{
	
	glPushMatrix();{
		
				glTranslatef(-65,-52.5,32);
			GLUquadricObj *obj = gluNewQuadric();
			glColor3f(1.0,1.0,1.0);
			gluCylinder(obj,3,3.5,4,15,15);
			}glPopMatrix();
	glColor3f(0.1,0.1,0.0);
	glTranslatef(-65,-52,36);
	glScalef(8,8,2);
	glutSolidCube(1);
	}glPopMatrix();

// up cube and cyl6
glPushMatrix();{
	
	glPushMatrix();{
		
				glTranslatef(-40,-52.5,32);
			GLUquadricObj *obj = gluNewQuadric();
			glColor3f(1.0,1.0,1.0);
			gluCylinder(obj,3,3.5,4,15,15);
			}glPopMatrix();
	glColor3f(0.1,0.1,0.0);
	glTranslatef(-40,-52,36);
	glScalef(8,8,2);
	glutSolidCube(1);
	}glPopMatrix();

//up cube and cyl5
glPushMatrix();{
	
	glPushMatrix();{
		
				glTranslatef(-15,-52.5,32);
			GLUquadricObj *obj = gluNewQuadric();
			glColor3f(1.0,1.0,1.0);
			gluCylinder(obj,3,3.5,4,15,15);
			}glPopMatrix();
	glColor3f(0.1,0.1,0.0);
	glTranslatef(-15,-52,36);
	glScalef(8,8,2);
	glutSolidCube(1);
	}glPopMatrix();

//up cube and cyl4
glPushMatrix();{
	
	glPushMatrix();{
		
				glTranslatef(15,-52.5,32);
			GLUquadricObj *obj = gluNewQuadric();
			glColor3f(1.0,1.0,1.0);
			gluCylinder(obj,3,3.5,4,15,15);
			}glPopMatrix();
	glColor3f(0.1,0.1,0.0);
	glTranslatef(15,-52,36);
	glScalef(8,8,2);
	glutSolidCube(1);
	}glPopMatrix();

//up cube and cyl3
glPushMatrix();{
	
	glPushMatrix();{
		
				glTranslatef(40,-52.5,32);
			GLUquadricObj *obj = gluNewQuadric();
			glColor3f(1.0,1.0,1.0);
			gluCylinder(obj,3,3.5,4,15,15);
			}glPopMatrix();
	glColor3f(0.1,0.1,0.0);
	glTranslatef(40,-52,36);
	glScalef(8,8,2);
	glutSolidCube(1);
	}glPopMatrix();

//up cube and cyl2
glPushMatrix();{
	
	glPushMatrix();{
		
				glTranslatef(65,-52.5,32);
			GLUquadricObj *obj = gluNewQuadric();
			glColor3f(1.0,1.0,1.0);
			gluCylinder(obj,3,3.5,4,15,15);
			}glPopMatrix();
	glColor3f(0.1,0.1,0.0);
	glTranslatef(65,-52,36);
	glScalef(8,8,2);
	glutSolidCube(1);
	}glPopMatrix(); 

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}

void animate(){
	//codes for any changes in Models, Camera
	
	//cameraAngle += cameraAngleDelta;	// camera will rotate at 0.002 radians per frame.
	
	//codes for any changes in Models

	//MISSING SOMETHING? -- YES: add the following
	glutPostRedisplay();	//this will call the display AGAIN

}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':	//reverse the rotation of camera
			cameraAngleDelta = -cameraAngleDelta;
			break;

		case '2':	//increase rotation of camera by 10%
			cameraAngleDelta *= 1.1;
			break;

		case '3':	//decrease rotation
			cameraAngleDelta /= 1.1;
			break;

		case '8':	//toggle grids
			canDrawGrid = 1 - canDrawGrid;
			break;
			
		case 'p':	//toggle grids
			cameraAngleDelta = 0;
			break;
		case 'r':	//toggle grids
			cameraAngleDelta = .001;
			break;		

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraRadius += 10;
			break;
		case GLUT_KEY_UP:		// up arrow key
			if(cameraRadius > 10)
				cameraRadius -= 10;
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.01;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.01;
			break;

		case GLUT_KEY_PAGE_UP:
			cameraHeight += 10;
			break;
		case GLUT_KEY_PAGE_DOWN:
			cameraHeight -= 10;
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				cameraAngleDelta = -cameraAngleDelta;	
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void init(){
	//codes for initialization
	loadImage();
	cameraAngle = 0;	//angle in radian
	cameraAngleDelta = 0.001;
	canDrawGrid = 1;

	cameraHeight = 50;
	cameraRadius = 150;
	//clear the screen
	glClearColor(0,0,0, 0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);
	
	//initialize the matrix
	glLoadIdentity();

	/*
		gluPerspective() — set up a perspective projection matrix

		fovy -         Specifies the field of view angle, in degrees, in the y direction.
        aspect ratio - Specifies the aspect ratio that determines the field of view in the x direction. The aspect ratio is the ratio of x (width) to y (height).
        zNear -        Specifies the distance from the viewer to the near clipping plane (always positive).
        zFar  -        Specifies the distance from the viewer to the far clipping plane (always positive).
    */
	
	gluPerspective(70,	1,	0.1,	10000.0);
	
}

int main(int argc, char **argv){
	
	glutInit(&argc,argv);							//initialize the GLUT library
	
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	
	/*
		glutInitDisplayMode - inits display mode
		GLUT_DOUBLE - allows for display on the double buffer window
		GLUT_RGBA - shows color (Red, green, blue) and an alpha
		GLUT_DEPTH - allows for depth buffer
	*/
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	

	glutCreateWindow("Some Title");

	init();						//codes for initialization

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	//ADD keyboard listeners:
	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);

	//ADD mouse listeners:
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
