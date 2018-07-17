#include "stdafx.h"
#include "ToMeshWithRGB.h"
void plytomesh(string plyname) {
    pcl::PCLPointCloud2 clod;
    pcl::PLYReader reader;
    reader.read(plyname, clod);
    pcl::PCDWriter writer;
    writer.writeASCII("temporary.pcd", clod);
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZRGB>);
    if (pcl::io::loadPCDFile("temporary.pcd", *cloud)==-1)
        {
        PCL_ERROR("打不开pcd文件。\n");
        return ;
        }
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr filtered(new pcl::PointCloud<pcl::PointXYZRGB>());
    pcl::PassThrough<pcl::PointXYZRGB> filter;
    filter.setInputCloud(cloud);
    filter.filter(*filtered);
    pcl::NormalEstimationOMP<pcl::PointXYZRGB, pcl::Normal> ne;//计算点云法向
    pcl::search::KdTree<pcl::PointXYZRGB>::Ptr tree1(new pcl::search::KdTree<pcl::PointXYZRGB>);
    pcl::PointCloud<pcl::Normal>::Ptr cloud_normals(new pcl::PointCloud<pcl::Normal>());
    tree1->setInputCloud(filtered);
    ne.setNumberOfThreads(32);
    ne.setInputCloud(filtered);
    ne.setSearchMethod(tree1);
    ne.setKSearch(5);//k取得过大会失真
    ne.compute(*cloud_normals);
    //显示中心
    Eigen::Vector4f centroid;
    compute3DCentroid(*filtered, centroid);//计算点云中心
    ne.setViewPoint(centroid[0], centroid[1], centroid[2]);//将向量计算原点置于点云中心
                                                           //融合RGB点云和法向,将点云法向信息叠加在原点云，Normal表示以及法线坐标和曲面曲率估计
    pcl::PointCloud<pcl::PointXYZRGBNormal>::Ptr cloud_smoothed_normals(new pcl::PointCloud<pcl::PointXYZRGBNormal>());
    concatenateFields(*filtered, *cloud_normals, *cloud_smoothed_normals);
    //泊松重建
    pcl::search::KdTree<pcl::PointXYZRGBNormal>::Ptr tree2(new pcl::search::KdTree<pcl::PointXYZRGBNormal>);
    tree2->setInputCloud(cloud_smoothed_normals);
    pcl::Poisson<pcl::PointXYZRGBNormal> pn;
    pn.setSearchMethod(tree2);
    pn.setInputCloud(cloud_smoothed_normals);
    pn.setConfidence(false);//对法线进行归一化处理，true用法线向量长度作为置信度信息
    pn.setManifold(false);//true对多边形进行细分三角话时添加重心，设置false则不添加
    pn.setOutputPolygons(false);//设置是否输出为多边形
    pn.setIsoDivide(8);
    pn.setSamplesPerNode(3);//设置每个八叉树节点上最少采样点数目
    pcl::PolygonMesh mesh;//形成mesh网格
    pn.reconstruct(mesh);
    //显示无色模型
    //boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer0(new pcl::visualization::PCLVisualizer("3D模型"));
    //viewer0->setBackgroundColor(255, 255, 255);
    //viewer0->addPolygonMesh(mesh, "my");
    //viewer0->initCameraParameters();
    //while (!viewer0->wasStopped())
    //    {
    //    viewer0->spinOnce(100);
    //    boost::this_thread::sleep(boost::posix_time::microseconds(100000));
    //    }
    //泊松重建完成
    //给mesh染色
    pcl::PointCloud<pcl::PointXYZRGB> cloud_color_mesh;
    pcl::fromPCLPointCloud2(mesh.cloud, cloud_color_mesh);//mesh信息存入点云
    pcl::KdTreeFLANN<pcl::PointXYZRGB> kdtree;//使用kdtree将原点云的信息映射在无颜色mesh上，并生成彩色mesh
    kdtree.setInputCloud(cloud);
    //k近邻算法
    //k越小越需要避免噪点，这就看前面的了
    int K=2;
    std::vector<int> pointIdxNKNSearch(K);
    std::vector<float> pointNKNSquaredDistance(K);
    for (int i=0; i<cloud_color_mesh.points.size(); ++i)
        {
        uint8_t r=0;
        uint8_t g=0;
        uint8_t b=0;
        float dist=0.0;
        int red=0;
        int green=0;
        int blue=0;
        //uint32_t rgb;
        if (kdtree.nearestKSearch(cloud_color_mesh.points[i], K, pointIdxNKNSearch, pointNKNSquaredDistance)>0)
            {
            for (int j=0; j<pointIdxNKNSearch.size(); ++j)
                {
                r=cloud->points[pointIdxNKNSearch[j]].r;
                g=cloud->points[pointIdxNKNSearch[j]].g;
                b=cloud->points[pointIdxNKNSearch[j]].b;
                red+=int(r);
                green+=int(g);
                blue+=int(b);
                dist+=1.0/pointNKNSquaredDistance[j];
                }
            }
        cloud_color_mesh.points[i].r=int(red/pointIdxNKNSearch.size()+0.5);
        cloud_color_mesh.points[i].g=int(green/pointIdxNKNSearch.size()+0.5);
        cloud_color_mesh.points[i].b=int(blue/pointIdxNKNSearch.size()+0.5);
        }
    //点云信息存入mesh，即rgb信息
    toPCLPointCloud2(cloud_color_mesh, mesh.cloud);
    boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer(new pcl::visualization::PCLVisualizer("3D模型上色版"));
    viewer->setBackgroundColor(255, 255, 255);
    viewer->addPolygonMesh(mesh, "my");
    viewer->initCameraParameters();
    while (!viewer->wasStopped())
        {
        viewer->spinOnce(100);
        boost::this_thread::sleep(boost::posix_time::microseconds(100000));
        }
    return ;
    //-----------------------------------------------------------------------------------------------------------
    //方法二
    //pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
    //if (pcl::io::loadPCDFile("l.pcd", *cloud)==-1)
    //    {
    //    PCL_ERROR("打不开pcd文件。\n");
    //    return 0;
    //    }
    //pcl::PointCloud<pcl::Normal>::Ptr cloud_normals(new pcl::PointCloud<pcl::Normal>());
    //pcl::IntegralImageNormalEstimation<pcl::PointXYZ, pcl::Normal> ne;
    //ne.setNormalEstimationMethod(ne.COVARIANCE_MATRIX);
    //ne.setMaxDepthChangeFactor(0.02f);
    //ne.setNormalSmoothingSize(10.0f);
    //ne.setInputCloud(cloud);
    //ne.compute(*cloud_normals);
    //////显示中心
    //pcl::visualization::PCLVisualizer viewer("PCL Viewer");
    //viewer.setBackgroundColor(0.0, 0.0, 0.5);
    //viewer.addPointCloudNormals<pcl::PointXYZ, pcl::Normal>(cloud, cloud_normals);
    //while (!viewer.wasStopped())
    //    {
    //    viewer.spinOnce();
    //    }
    //return 0;
    }