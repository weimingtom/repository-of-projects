import glapp.GLApp;

import java.awt.Color;
import java.awt.FontFormatException;
import java.io.IOException;

import org.lwjgl.LWJGLException;
import org.lwjgl.Sys;
import org.lwjgl.input.Keyboard;
import org.lwjgl.opengl.Display;
import org.lwjgl.opengl.DisplayMode;
import org.lwjgl.opengl.GL11;
import org.lwjgl.opengl.GL12;
import org.lwjgl.util.glu.GLU;
import org.newdawn.slick.SlickException;
import org.tinder.studio.lwjgl.heightmap.HeightMap;
import org.tinder.studio.lwjgl.util.Point3f;

import de.bloodyblades.ms3dloader.Font;

/**
 *
 * @Author Micheal Hong
 * @Email babala_234@163.com
 * @Version 2010-7-1����07:45:35
 *
 */
public class SceneDemo{
	
	private static ResourceLoader resourceLoader = new ResourceLoader();
	public static final int DISPLAY_WIDTH=300;
	public static final int DISPLAY_HEIGHT=200;
	
	private boolean runnable=true;
	private static final int FPS=50;
	
	private static Font font=null;
	
	private HeightMap heightMap;
	private Point3f[][] strips;
	
	public SceneDemo() throws LWJGLException, IOException{
		this.init();
		this.run();
	}

	private void init() throws LWJGLException, IOException
	{
		Display.setDisplayMode(new DisplayMode(DISPLAY_WIDTH, DISPLAY_HEIGHT));
		
		/*����*/
		int width=Display.getDesktopDisplayMode().getWidth();
		int height=Display.getDesktopDisplayMode().getHeight();
		int x=(width-DISPLAY_WIDTH)/2;
		int y=(height-DISPLAY_HEIGHT)/2;
		Display.setLocation(x,y);
		
		/*ȫ��*/
//		Display.setFullscreen(true);
		
		Display.create();
		Display.setTitle("BaseDemo");
		
		GL11.glViewport(0,0,DISPLAY_WIDTH,DISPLAY_HEIGHT);
		
		//��������ͶӰ
		GL11.glMatrixMode(GL11.GL_PROJECTION);
    	GL11.glLoadIdentity();
//    	GL11.glOrtho(0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0, 0,25);
    	GLU.gluPerspective(90.0f,(float)width/(float)height,0.1f,100.0f);
    	
    	GL11.glEnable(GL11.GL_BLEND);
    	GL11.glEnable(GL11.GL_ALPHA_TEST);
    	GL11.glAlphaFunc(GL11.GL_GREATER, 0);
    	
		font = new Font(resourceLoader.loadResourceAsStream("textures/font.bmp"), 16, width, height);
		heightMap=new HeightMap(80,80,resourceLoader.loadResourceAsStream("textures/heightmap.png"));
		strips=heightMap.generateTriangleStrip();
		
		GL11.glMatrixMode(GL11.GL_MODELVIEW);
	}
	
	private void run(){
		while (runnable) {
			long startTime = System.currentTimeMillis();
			GL11.glClearColor(0,0,0,1);
			GL11.glClear(GL11.GL_COLOR_BUFFER_BIT| GL11.GL_DEPTH_BUFFER_BIT);
			
			render();
			
			if (Keyboard.next() && Keyboard.getEventKeyState()&&Keyboard.getEventKey()==Keyboard.KEY_ESCAPE||Display.isCloseRequested()) {
					runnable=false;
					break;
			}
            long costTime = System.currentTimeMillis() - startTime;
            if(costTime<1000/FPS)
            {
                try{
                  Thread.sleep(1000/FPS-costTime);
                }
                catch(Exception e){
                   e.printStackTrace();
                }
            }
            GL11.glColor3f(0,1,0);
            font.print("FPS:"+String.valueOf(costTime==0?FPS:1000/costTime),5,22,0);
            Display.update();
		}
		Keyboard.destroy();
		Display.destroy();
		System.out.println("Program stopped");
	}
	
	private void render() {
		GL11.glClearColor(0.5f,0.5f,0.5f, 1f);
		GL11.glClear(GL11.GL_COLOR_BUFFER_BIT|GL11.GL_DEPTH_BUFFER_BIT);

		
		GL11.glLoadIdentity();
		GL11.glTranslated(-50,-50, -100);
		for(int i=0;i<this.strips.length;i++)
		{
			GL11.glBegin(GL11.GL_TRIANGLE_STRIP);
			for(int j=0;j<this.strips[i].length;j++)
			{
				Point3f point=this.strips[i][j];
				GL11.glVertex3f(point.x,point.z,point.y);
				System.out.println(point.x+","+point.y+","+point.z);
			}
			GL11.glEnd();
		}
		

//		GL11.glEnableClientState(GL11.GL_VERTEX_ARRAY);
//		GL11.glEnableClientState(GL11.GL_NORMAL_ARRAY);
//		GL11.glEnableClientState(GL11.GL_TEXTURE_COORD_ARRAY);
//		GL11.glEnable(GL11.GL_TEXTURE_2D);
//		GL11.glDrawArrays(GL11.GL_TRIANGLES, 0, 3);
//		GL11.glDisable(GL11.GL_TEXTURE_2D);
//		GL11.glDisableClientState(GL11.GL_VERTEX_ARRAY);
//		GL11.glDisableClientState(GL11.GL_NORMAL_ARRAY);
//		GL11.glDisableClientState(GL11.GL_TEXTURE_COORD_ARRAY);
	}
	
	public static void main(String[] args) throws LWJGLException, IOException
	{
		new SceneDemo();
	}

}