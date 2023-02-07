Discription of the task: 
 
Making a ROS Node which takes in PointCloud2 data, applies ground plane elimination and segmentation, and publishes it on another topic.
 
Description of the ROS Nodes, Topics and Messages used:
a single node which converts the ROS Message type to a PCL message and processes the data is used. Topics used are the data in which the kinect of the rover publishes data and also the adverticed topic "/pcl_pointcloud". Message type published is of type 'sensor_msgs::PointCloud2'
 
RQT Graph: 
 
![pcl](https://user-images.githubusercontent.com/88363398/174494898-225bff83-5b5b-4060-90d0-ab3cc74a6ebd.png) 
 
Rviz Visualization of Clusters: 
 
![Screenshot from 2022-06-19 23-38-26](https://user-images.githubusercontent.com/88363398/174494914-de020700-640a-46b9-b66d-93b870aaae34.png)
 
Picture of vizualised oobjects in simulation: 
 
![Screenshot from 2022-06-19 23-38-42](https://user-images.githubusercontent.com/88363398/174494962-12b1e71d-284e-42df-904f-a0aba10b469b.png)

