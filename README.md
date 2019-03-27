# RealtimeStitch实时视频拼接

####  项目背景
	从连接的多台摄像机获取监控画面，拼接生成全景图；
	VC++ 项目，使用virtual studio 2013开发，依赖Opencv2和vlc库，64位。
	这个工程是给算法同学调试用的，所以框架基本成型，但细节不太完整。
####  类图
![image](https://github.com/DanicaMeng/RealtimeStitch/raw/master/ClassDiagram.jpg)
####  目录说明
> frame：获取视频帧
> > 
> > FrameCapture：视频抓取父类，包含帧缓存队列（m_matQue)、向缓存放入帧（pushFrame）、获取帧（next）；
> > 
> > RTSPCapture：继承FrameCapture，通过Opencv的VideoCapture对象打开视频流，建立线程获取视频帧，然后放入缓存队列；
> > 
> > VLCRTSPCaptrue：继承FrameCapture，通过VLC的函数打开视频流，并接受视频帧放入缓存队列，建立线程用于监测网络情况；
> > 
> > CaptureManager：创建和初始化帧抓取对象的单例类，提供获取抓取对象列表、帧列表方法；
>  
> 
> stitch：视频拼接
> > 
> > MatStitcher：视频帧拼接接口；
> > 
> > TestStitcher：继承MatStitcher，测试用的，直接返回参数中的第一个mat作为拼接结果，
> > 
> > OpencvStitcher:继承MatStitcher，调用的Opencv的stitcher类实现拼接。优点：图像个数、大小、顺序任意。缺点：速度慢，不是线程安全的，不能多个线程同时执行合并。
> > 
> > SurfStitcher: 继承MatStitcher，网上找的的图片拼接源码，出处已经不可查。优点：速度快。缺点：只支持两幅图左右拼接，先右后左；效果一般；
> > 
> > StitcherManager：创建和初始化拼接对象的单例类，提供拼接调用函数；
> > 
> > 注：真正的拼接算法从Opencv的stitcher源码中改的，不能放进来了。需要满足：线程安全，图像个数和顺序任意，速度和拼接效果满足需求；
>
>
> control： 控制
> > 
> > threadpool：线程池
> > 
> > > ExecuteThread：线程池中的执行线程。无任务时等待；设置任务（setParam）后，执行任务（ThreadPoolTask的execute接口），执行完继续等待；
> > >
> > > ThreadPool：线程池，创建、销毁执行线程；收到任务（addjob）时，选择空闲线程执行，没有空闲线程，则一直等到有执行线程空闲。
> > >
> > > ThreadPoolTask: 线程任务接口。
> > 
> > ComposeManager：创建线程池；在新的线程中，获取视频帧，创建任务，给线程池执行；管理任务，提供获取结果的接口。
> > 
> > ComposeTask：继承ThreadPoolTask，实现任务接口（execute）；提供任务参数，和保持结果。
	