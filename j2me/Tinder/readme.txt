
/*===============M3G相关===============*/
3D编程指南第一部分：快速进入移动JAVA 3D编程世界
	http://www.99inf.net/SoftwareDev/Java/48840.htm
3D编程指南第二部分：光的3D理论与定位
	http://www.99inf.net/SoftwareDev/Java/48841.htm
3D编程指南第三部分：粒子系统和立即模式渲染(1)
	http://www.99inf.net/SoftwareDev/Java/48847.htm
3D编程指南第三部分：粒子系统和立即模式渲染(2)
	http://www.99inf.net/SoftwareDev/Java/48848.htm
3D编程指南第四部分：M3G内建碰撞、光照物理学和照相机视点
	http://www.52rd.com/Blog/Detail_RD.Blog_juney_3078.html
3D编程指南第五部分：使用M3G渲染heightmap地形
	http://www.gz183.com.cn/Info/193/info36030_1.htm
What's the unit length in m3g
	http://blog.csdn.net/xueyong1203/archive/2007/03/12/1526760.aspx
Export model (teapot4_background.max) to m3g file.
	http://www.mobilefish.com/tutorials/3dsmax/3dsmax_quickguide_export_m3g_4.html
获得.m3g文件里面的物体的位置
	Transform _tPosition;
	_mesh.getTransformTo(_world, _tPosition);
	_world是这个.m3g的world，如果返回true，则_mesh的位置就会储存在_tPosition里面了
M3G内坐标系作用范围的混淆
	http://blog.csdn.net/xueyong1203/archive/2007/01/05/1474459.aspx
Rendering world后的内存的释放
	http://blog.csdn.net/xueyong1203/archive/2007/01/05/1474457.aspx
getOrientation 得到的数据的意义
	public void getOrientation(float[] angleAxis)
	得到
	angleAxis[0] = 38.0f  angleAxis[1] = 12 angleAxis[2] = 30, angleAxis[2] = 18
	The getOrientation() method retrieves the current orientation component of the Transformable instance in question. The first item in the returned array is the angle in degrees and the three others define the axis of rotation. 
	The same from another perspective: The object on which you called the method, is first transformed by the free used transform, then scaled by some amount, then rotated 38 degrees around axis  (12, 30, 18) and then translated with some vector. 
	本文来自CSDN博客，转载请标明出处：http://blog.csdn.net/xueyong1203/archive/2007/01/05/1474454.aspx
Mesh 的 duplicate()方法
	http://blog.csdn.net/xueyong1203/archive/2007/01/05/1474437.aspx
Transform.transform(float[] vectors)的物理意义
	http://blog.csdn.net/xueyong1203/archive/2007/01/05/1474435.aspx
Transform.setIdentity()&invert()&transpose()
	http://blog.csdn.net/xueyong1203/archive/2007/01/05/1474421.aspx
手动替换/更新模型的纹理
	  ((Mesh) nodeUnit).getAppearance(0).getTexture(0).setImage(img2dSequences[indexFrame]);
终于实现Object lookAt Object了。。
	http://blog.csdn.net/xueyong1203/archive/2007/01/05/1474407.aspx
翻译的Mesh类
	http://blog.csdn.net/xueyong1203/archive/2007/01/04/1473514.aspx
我翻译的Material 
	http://blog.csdn.net/xueyong1203/archive/2007/01/04/1473510.aspx
M3G优化技巧 持续更新中。。
	http://blog.csdn.net/xueyong1203/archive/2007/01/04/1473481.aspx
ClausHoefele.com - M3G(M3G教程)
	http://www.claushoefele.com/m3g/examples.html
	

	
	
/*===============网络协议相关===============*/
网络游戏的位置同步
	http://blog.csdn.net/lfhfut/archive/2008/03/20/2198600.aspx
网络游戏中的数据包设计与定义（初论）
	http://blog.csdn.net/sodme/archive/2004/06/23/22958.aspx
拼包函数及网络封包的异常处理(含代码)
	http://blog.csdn.net/sodme/archive/2005/07/10/419233.aspx
逆向思维----魔兽世界封包分析(2)
	http://blog.csdn.net/sodme/archive/2005/07/10/419359.aspx
逆向思维--魔兽世界封包分析(1)
	http://blog.csdn.net/sodme/archive/2005/06/18/397371.aspx
网络游戏的对时以及同步问题
	http://blog.codingnow.com/2006/04/sync.html
貌似合理的网络包协议
	http://blog.codingnow.com/2006/01/aeeieaiaeioeacueoe.html#comments
.net服务器向j2me客户端发送字节流，在穿过cmwap网-关时字节流的内容发生变化，为什么？
 	http://blog.csdn.net/xueyong1203/archive/2007/03/28/1543796.aspx
	
/*===============声音相关===============*/
混音算法的学习与研究 
	http://www.cppblog.com/jinq0123/archive/2007/10/31/audiomixingstudy.html
软件混音的实现 
	http://blog.csdn.net/tangl_99/archive/2005/11/07/524647.aspx
S60 Platform: Sound Mixer Example v2.0
	http://www.forum.nokia.com/info/sw.nokia.com/id/7bbe94f9-119c-475e-942b-aa248d012544/S60_Platform_Sound_Mixer_Example_v2_0.zip.html
	
/*===============2D计算相关===============*/
贝塞尔曲线(Bezier Curves) 2d实现
	http://www.azure.com.cn/article.asp?id=299
Calculating the area and centroid of a polygon(多边形的面积和重心计算)
	http://local.wasp.uwa.edu.au/~pbourke/geometry/polyarea/
向量几何在游戏编程中的使用4——2-D物体间的碰撞响应
	http://blog.csdn.net/xueyong1203/archive/2008/01/10/2033861.aspx
 向量几何在游戏编程中的使用3——2-D边界碰撞检测
 	http://blog.csdn.net/xueyong1203/archive/2008/01/10/2033858.aspx
 向量几何在游戏编程中的使用2——2-D物体任意角度的反弹
	http://blog.csdn.net/xueyong1203/archive/2008/01/10/2033855.aspx
向量几何在游戏编程中的使用1——简单的2-D追踪
	http://blog.csdn.net/xueyong1203/archive/2008/01/10/2033850.aspx
2D碰撞教程2：像素检测
	http://blog.csdn.net/vinile/archive/2007/07/27/1712093.aspx
Ray casting in a 2D tile-based environment(2D地形)
	http://www.codeproject.com/KB/GDI-plus/RayCasting2D.aspx
训练三－卡马克算法（郑鸿）
	http://blog.csdn.net/ok9034875/archive/2007/04/23/1576589.aspx
精髓游戏算法收拾(A*算法)
	http://www.j2mehome.com/j2me/jinjie/12898.html
An Oriented Bounding Box (OBB) Intersection Test 
	http://blog.csdn.net/xueyong1203/archive/2007/01/23/1491368.aspx
 A* 算法的一些文章集合
 	http://blog.csdn.net/xueyong1203/archive/2006/05/08/713622.aspx
 随机迷宫生成算法浅析
 	http://www.j2megame.org/index.php/content/view/2120/125.html
 随机生成迷宫算法及其 Java 实现
 	http://www.robotsky.com/e/DoPrint/?classid=32&id=3986
 一个JAVA编写的迷宫算法。。自动找迷宫出口
 	http://blog.sina.com.cn/s/blog_60c11a490100dwak.html
	
	

/*===============3D计算相关===============*/
关于任意轴旋转的编程与实践 
	http://blog.csdn.net/xueyong1203/archive/2008/01/10/2033880.aspx
深入探索透视投影变换
	http://blog.csdn.net/xueyong1203/archive/2008/01/10/2033873.aspx
向量几何在游戏编程中的使用6——3-D空间中的基变换与坐标变换
	http://blog.csdn.net/xueyong1203/archive/2008/01/10/2033866.aspx
向量几何在游戏编程中的使用5 ——物体的旋转
	http://blog.csdn.net/xueyong1203/archive/2008/01/10/2033864.aspx
矩阵求逆的一种优化算法(C)
	http://www.cnitblog.com/lethep/archive/2007/08/15/31881.html
小谈矩阵和坐标变换
	http://blog.5d.cn/user39/myasuka2000/200711/451102.html
什么是仿射变换以及仿射变换矩阵？
	http://blog.csdn.net/ok9034875/archive/2007/04/24/1578259.aspx
网游中人物的碰撞后反应控制
	http://www.cnblogs.com/fishboy82/archive/2008/03/01/1087553.html
如何沿墙壁滑动
	http://www.azure.com.cn/article.asp?id=217
An OBB-Line Segment Test (Testing a box and a line segment for intersection requires checking only six separating axes
	http://blog.csdn.net/xueyong1203/archive/2007/03/27/1542542.aspx)
 深入探讨透视投影坐标变换
 	http://blog.csdn.net/xueyong1203/archive/2007/03/23/1538742.aspx
 3D数学与游戏：四元数与旋转 
 	http://blog.csdn.net/xueyong1203/archive/2007/02/15/1510470.aspx
 四元数(Quaternions) 
 	http://blog.csdn.net/xueyong1203/archive/2007/02/15/1510467.aspx
 探讨：物体绕任意向量的旋转-四元数法VS.旋转矩阵法的性能比较 
 	http://www.cnblogs.com/soroman/archive/2006/09/19/509597.html
 使JSR184相机绕世界坐标系中的坐标轴旋转！
 	http://blog.csdn.net/xueyong1203/archive/2007/02/08/1505406.aspx
变换AABB的快速算法
	http://blog.csdn.net/xueyong1203/archive/2007/02/06/1503605.aspx
高级碰撞检测技术
	http://blog.csdn.net/lookus/archive/2006/05/06/709957.aspx
J2ME中的基础碰撞检测算法
	http://blog.csdn.net/Mailbomb/archive/2005/10/22/513675.aspx
AABB Trees: Best Axis Algorithm
	http://www.cyberkreations.com/kreationsedge/?page_id=27
Fast (!) and correct Frustum - AABB Intersection
	http://www.gamedev.net/community/forums/topic.asp?topic_id=512123&forum_id=20&gforum_id=0
View Frustum Culling Tutorial
	http://zach.in.tu-clausthal.de/teaching/cg_literatur/lighthouse3d_view_frustum_culling/index.html
在游戏开发中如何用高级碰撞检测技术 
	http://game.chinaitlab.com/program/30691.html
基于2D多边形的碰撞检测和响应(一)
	http://blog.csdn.net/nette/archive/2008/02/27/2124894.aspx

/*===============3D换装相关===============*/
3D换装资料搜集
	http://blog.csdn.net/xueyong1203/archive/2009/02/13/3887727.aspx
[美术制作流程]魔兽世界角色换装分析 
	http://www.cnblogs.com/winsonchen/archive/2008/01/24/1051716.html
构建地形系统（翻译）2
	http://www.sf.org.cn/design/Design_Tutorial/design_19214.html
	
/*===============地形相关===============*/
大规模草地的快速绘制排序
	http://blog.csdn.net/xueyong1203/archive/2008/11/27/3390376.aspx
Terrain Geomorphing in the Vertex Shader 
	http://www.gamedev.net/reference/articles/article1936.asp
[室外地形专辑]技术概览
	http://www.cnblogs.com/winsonchen/archive/2008/02/05/1064793.html
没有贴图拉伸的陡峭悬崖
	http://www.cnblogs.com/cproom/archive/2006/08/07/470066.html
魔兽3自适应地块贴图算法
	http://blog.csdn.net/xueyong1203/archive/2007/08/30/1765475.aspx
使用Managed DirectX创建三维地形
	http://blog.csdn.net/soilwork/archive/2005/11/02/520962.aspx
3dsmax Heightmaps
	http://www.ogre3d.org/wiki/index.php/3dsmax_Heightmaps
Terrain generation tools
	http://blog.csdn.net/xueyong1203/archive/2007/04/10/1559008.aspx
Terrain Generating
	http://dev.gameres.com/Program/Visual/3D/terain.htm
Slope(斜坡) 法线生成算法，在地形渲染中的应用(高度图中没有包含法线信息，无法进行光照)
	http://www.azure.com.cn/article.asp?id=285
VOXEL 技术在即时渲染地形中的应用 by 樊一鹏
	http://blog.csdn.net/xueyong1203/archive/2007/01/22/1489687.aspx
 3D地表生成及渲染 (VOXEL) by 云风
 	http://blog.csdn.net/xueyong1203/archive/2007/01/22/1489682.aspx
 四叉树 Quadtrees
 	http://blog.csdn.net/xueyong1203/archive/2007/01/22/1489678.aspx
	
/*===============灯光相关===============*/
Light Mapping - Theory and Implementation
	http://www.flipcode.com/archives/Light_Mapping_Theory_and_Implementation.shtml
M3G embedded lightmaps
	http://blog.csdn.net/xueyong1203/archive/2007/03/21/1535996.aspx
	
	
/*===============鼠标拾取相关===============*/
Mouse Picking Demystified
	http://trac.bookofhook.com/bookofhook/trac.cgi/wiki/MousePicking
	
	
/*===============角色动画相关===============*/
近期在研究Skeletal Animation（骨骼动画）
	http://blog.csdn.net/skyman_2001/archive/2006/05/23/750893.aspx
[原创] 骨骼运动变换的数学计算过程详解
	http://www.cnblogs.com/neoragex2002/archive/2007/09/13/Bone_Animation.html
3D游戏角色动画原理及应
	http://blog.csdn.net/xueyong1203/archive/2007/03/22/1537798.aspx
	

/*===============Alpha相关===============*/
Alpha混合物体的深度排序
	http://blog.csdn.net/xoyojank/archive/2009/02/21/3918091.aspx
Alpha transparency tutorial
	http://www.terathon.com/wiki/index.php?title=Alpha_transparency
W810 compositing mode alpha bug 
	http://blog.csdn.net/xueyong1203/archive/2007/08/31/1766515.aspx
看到一片讨论ALPHA BLENDING的帖子 
	http://blog.csdn.net/xueyong1203/archive/2006/05/08/712637.aspx
	
	
/*===============移植相关===============*/
使Java代码可移植之一
	http://www.99inf.net/SoftwareDev/Java/48844.htm
使Java代码可移植之二
	http://www.99inf.net/SoftwareDev/Java/48845.htm

/*===============粒子相关===============*/
MIDP中尝试粒子系统
	http://www.99inf.net/SoftwareDev/Java/48846.htm
	

/*===============蓝牙相关===============*/
使用Java蓝牙无线通讯技术API（第一部分 -API概览）（1）
	http://www.99inf.net/SoftwareDev/Java/48853.htm
使用Java蓝牙无线通讯技术API（第一部分 -API概览）（2）
	http://www.99inf.net/SoftwareDev/Java/48854.htm
[ZT] 蓝牙连接code
	http://blog.csdn.net/xueyong1203/archive/2008/06/05/2513104.aspx
使用J2ME搜索蓝牙设备，并与搜到的设备通信
	http://blog.csdn.net/hellogv/archive/2008/04/24/2323313.aspx
		

/*===============数值相关===============*/
魔兽争霸3中的一些数学计算
	http://blog.csdn.net/xueyong1203/archive/2009/07/22/4370625.aspx
	
/*===============工具相关===============*/
j2mepolish研究报告
	http://huohoustudio.spaces.live.com/blog/cns!D265DB37F9B75BA2!310.entry
Real - Java floating point library for MIDP devices
	http://real-java.sourceforge.net/Real.html
Apache MINA 
	http://mina.apache.org/
决定尝试下JOGL！（3D编辑器参考）
	http://blog.csdn.net/xueyong1203/archive/2007/06/27/1667903.aspx
ProGuard GUI
	http://proguard.sourceforge.net/
OZCollide(一个很好的高效碰撞引擎)
	http://www.tsarevitch.org/ozcollide/
·【教程】教你如何截取3D网游模型
	http://bbs.gameres.com/showthread.asp?threadid=141970
xna fbx模型导出插件。Autodesk公司好牛啊！
	http://usa.autodesk.com/adsk/servlet/pc/item?siteID=123112&id=10775855
3D Studio Max SMD Import Plug-in - Import SMD mode 
	http://www.moddb.com/games/half-life/downloads/3d-studio-max-smd-import-plug-in-import-smd-mode

/*===============XML相关===============*/
在以CLDC為基礎的架構上分析XML 
	http://blog.csdn.net/xueyong1203/archive/2007/06/21/1661097.aspx
J2ME中使用KXML的例子
	http://blog.csdn.net/xueyong1203/archive/2007/06/21/1661084.aspx
KXML：J2ME中的XML语法分析利器
	http://blog.csdn.net/xueyong1203/archive/2007/06/21/1661068.aspx


/*===============手机参数查询===============*/
http://www.mobilade.com/ml/home
http://www.jbenchmark.com/result.jsp?orderby=date&brand=all&submit=OK&benchmark=3d
 手机性能参数及BUG总结(持续搜寻添加中....) 
 	http://blog.csdn.net/xueyong1203/archive/2006/07/28/991851.aspx
 各型号手机Bug总结
 	http://blog.csdn.net/xueyong1203/archive/2006/04/26/677924.aspx

/*===============脚本相关===============*/
再探游戏--天赐脚本
	http://blog.csdn.net/zdsdiablo/archive/2006/11/13/1381156.aspx

	
/*===============其他===============*/
即时战略游戏中如何协调对象移动
	http://dev.gameres.com/Program/Abstract/RTSMoveObject.htm
动态加载资源
	http://blog.codingnow.com/2006/01/dynamic_loading.html
Fast integer square roots in Java(求数值的根的几种方法)
	http://atoms.alife.co.uk/sqrt/index.html
完成端口(IOCP)编程探讨
	http://blog.csdn.net/xueyong1203/archive/2007/08/31/1767085.aspx
摩托手机上的音效开发问题
	http://blog.csdn.net/xueyong1203/archive/2007/05/29/1629105.aspx
图形图像处理－之－高质量的快速的图像缩放 下篇 三次线性插值和MipMap链
	http://blog.csdn.net/housisong/archive/2007/03/13/1528392.aspx
图形图像处理－之－高质量的快速的图像缩放 中篇 二次线性插值和三次卷积插值
	http://blog.csdn.net/housisong/archive/2006/12/21/1452249.aspx
图形图像处理－之－高质量的快速的图像缩放 上篇 近邻取样插值和其速度优化
	http://blog.csdn.net/housisong/archive/2006/11/22/1404896.aspx
图形图像处理－之－任意角度的高质量的快速的图像旋转 上篇 纯软件的任意角度的快速旋转
	http://blog.csdn.net/housisong/archive/2007/04/27/1586717.aspx
KVM的执行引擎（上） — 栈和帧 
	http://blog.csdn.net/xueyong1203/archive/2007/03/13/1528156.aspx
KVM的执行引擎（下） — 指令集
	http://blog.csdn.net/fyzhao/archive/2006/08/31/1150446.aspx
KVM的常量池
	http://blog.csdn.net/xueyong1203/archive/2007/03/13/1528168.aspx
 Can I display a Sprite3D object at 2 places in the world?
 	http://blog.csdn.net/xueyong1203/archive/2007/03/12/1527092.aspx
 Camera In JSR184 by Lyo Wu
 	http://blog.csdn.net/xueyong1203/archive/2007/02/01/1499730.aspx
 计算几何算法概览
 	http://blog.csdn.net/xueyong1203/archive/2007/01/22/1489691.aspx
 Camera.translate的陷阱！问题解决了
 	http://blog.csdn.net/xueyong1203/archive/2007/01/12/1481307.aspx
 M3G里面的BillBoard(有图)
 	http://blog.csdn.net/xueyong1203/archive/2007/01/10/1479299.aspx
 关于PNG图像压缩的一点感悟 by cocomo
 	http://blog.csdn.net/xueyong1203/archive/2007/01/09/1477750.aspx
 通用数据压缩算法简介
 	http://www.j2megame.org/index.php/content/view/1889/125.html
 对高速二次线性插值算法的讨论
 	http://blog.csdn.net/xueyong1203/archive/2007/01/04/1473420.aspx
 图像几何变换（缩放、旋转）中的插值算法
 	http://blog.csdn.net/xueyong1203/archive/2007/01/04/1473405.aspx
 基于矢量点积的高效可视剔除算法 
 	http://blog.csdn.net/xueyong1203/archive/2006/08/16/1075353.aspx
 [转帖][风过回廊的小屋]手机中文码制问题的一点理解- -
 	http://blog.csdn.net/xueyong1203/archive/2005/12/27/562897.aspx
J2ME游戏开发之免费地图编辑器(2D and 2.5D) 
 	http://blog.csdn.net/xueyong1203/archive/2005/11/19/532838.aspx
 J2ME游戏开发学习之用setClip分割图片
 	http://blog.csdn.net/xueyong1203/archive/2005/09/02/470186.aspx
 J2ME游戏开发学习之高效&漂亮显示积分
 	http://blog.csdn.net/xueyong1203/archive/2005/09/02/470115.aspx
 [翻译][难度2]教程：如何进行骨骼绑定（图片已修复）
 	http://bbs.mountblade.com.cn/viewthread.php?tid=3530&extra=page%3D1
 关于J2ME的中断事件处理 
 	http://www.j2megame.org/index.php/content/view/2275/125.html

 	

	
	
	
 3ds max坐标转换成opengl 坐标系
Max:   
Quat: (quat -0.772327 0.105251 0.0845889 0.620708)    
OGL: 
Quat:: -0.772327 , 0.084589 , -0.105251 , 0.620708   
Max:
Quat: (quat -0.772327 0.105251 0.0845889 0.620708) 
OGL:
Quat:: -0.772327 , 0.084589 , -0.105251 , 0.620708  

Max:   
Position: [-29.6717,-109.273,7.91964]   
OGL:   
Position:: -29.671656 , 7.919643 , 109.272964  
Max:
Position: [-29.6717,-109.273,7.91964]
OGL:
Position:: -29.671656 , 7.919643 , 109.272964

Max:   
Scale: [1.000000, 0.999999, 1.000000 ]   
OGL:   
Scale:: 1.000000 , 1.000000 , 0.999999   


Java Class File Major_Version 

Java SE 6 : 0x0032   
Java SE 5 : 0x0031 
JDK 1.4   : 0x0030 
JDK 1.3   : 0x002F
JDK 1.2   : 0x002E
JDK 1.1   : 0x002D 

preRotate vs postRotate
简单地讲，就是
preRotate先做其它的再旋转
而postRotate是先旋转再做其它的 

jsr184里面Light代价排序
none < emissive <=ambient << directional << spotlight 

Micro3D 里面的最大纹理尺寸
256x256 pixels(256 colors selected from a 16-bit color palette) 


  ARGB8888 --> ARGB4444&Other Formats 收藏 
int argb8888;
int a=(argb8888 & 0xff000000)>>24;
int r=(argb8888 & 0x00ff0000)>>16;
int g=(argb8888 & 0x0000ff00)>>8;
int b=(argb8888 & 0x000000ff)>>0;
 
short argb4444=((a>>4)<<12)|((r>>4)<<8)|((g>>4)<<4)|((b>>4)<<0);
short rgb555=((r>>3)<<10)|((g>>3)<<5)|((b>>3)<<0); 
short rgb565=((r>>3)<<11)|((g>>2)<<5)|(b>>3); 


Manifest.mf里面的自动换行问题
我们经常遇到因为MIDP-Description里面的字数太多而导致的mf文件自动换行，从而安装失败的问题
原来，根据
http://java.sun.com/j2se/1.3/docs/guide/jar/jar.html#Manifest%20Specification
描述的 Manifest规范，
mf文件里面每一行只能有72个bytes，去掉最开头的必须的一个冒号和空格
就剩下70个bytes了。。

  学好3D必备的数学基础 by TK 收藏 
Thanks TK!

学好3D必备的数学基础 by TK
坐标变换是把什么变换成什么？为什么要乘矩阵？矩阵的每行/列是什么含义？
乘矩阵的本质是什么？矩阵的行序/列序是什么意义？为什么DX和GL的矩阵是互相转置的？
矢量为什么要分1*4和4*1？矩阵的左乘和右乘有什么区别，各有什么含意？。。。
这些问题自己一定要搞清楚。
还有，为什么旋转/平移/缩放的顺序要这么排？如果颠倒顺序会如何？
为什么旋转一般按照ZXY的顺序进行？什么是万向锁。。。
查查资料吧，这些问题的答案如果是别人告诉你，那就没有意义了。  
