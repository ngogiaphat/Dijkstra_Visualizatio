#include<bits/stdc++.h>
#include<graphics.h>
#include<time.h>
#include<string>
using namespace std;
//vector<int> v[N] -- đó sẽ là một mảng gồm N đối tượng <int> vectơ khác nhau // mảng là 38 * 38 vectơ khác nhau
vector<int> adja[38 * 38];
int startx = 0, starty = 0, endx = 0 , endy = 0 , dis = 0;
int start_of_maxtrix_x = 10;
int start_of_maxtrix_y = 10;
int end_node_index = 0;
int gap_lines = 20;
int num_columns = 38;
int start_node_index = 0;
set<int>record_obstacles;
//chức năng này chuyển đổi nút đồ thị sang vị trí đồ họa để làm nổi bật
void nodes_to_map_points(int node,int color,int del)
{
	int y = node/38;
	int x = node%38;
	y = (y * 20) + 10;
	x = (x * 20) + 10; 
	setfillstyle(1,color); //được sử dụng để thay đổi màu sắc và kiểu kiểu được sử dụng
	delay(del);
	bar(x+5, y+5 , x + 15 , y + 15);
	setfillstyle(1,5);
	bar(endx+5, endy+5 , endx + 15 , endy + 15);	
}
//hàm này chuyển đổi vị trí con trỏ x, y thành nút thích hợp
int map_points_to_nodes(int x, int y , int gap_lines , int num_columns)
{
   x-=10;
   y-=10;
   x = x / gap_lines;
   y = y / gap_lines;
  // cout<<"Gap of Lines passed is "<<gap_lines<<" dabba is: "<<((y * num_columns) + x)<<endl;
   return ((y * num_columns) + x); 
}
void add_obstacles( int start, int end) //có bắt đầu và có kết thú, để không coi đó là nút chướng ngại vật
{
	//font,direction,character size
	settextstyle(1, HORIZ_DIR,2);
	//x coordinate, y coordinate , test To Display
	outtextxy(800, 20 + 20, "SELECT THE ");
	outtextxy(800, 20 + 20 + 20 + 10, "OBSTACLES");
	outtextxy(800, 20 + 20 + 20 + 20 + 20, "NODES    ");
	outtextxy(800, 20 + 20 + 20 + 20 + 60, "Right Click");
	outtextxy(800, 20 + 20 + 20 + 20 + 80, "To Finish");
	outtextxy(800, 20 + 20 + 20 + 20 + 100, "Obs. Selection");
	POINT cursorPosition;
	int mx = 0,my = 0;
	int count = 0;
	//map<int,int> mymap;
	while(1)
	{	 
		GetCursorPos(&cursorPosition);
	 	mx = cursorPosition.x - 10; //để thoát ra khỏi sự khác biệt
	 	my = cursorPosition.y - 30;  
	 //	cout<<mx<<" "<<my<<endl;
	 	if(GetAsyncKeyState(VK_LBUTTON))
		{
			int tempx = mx - start_of_maxtrix_x ;
		 	int tempy = my - start_of_maxtrix_y ;
			if(tempx >= gap_lines)
			{
				tempx = tempx / gap_lines;
			  	tempx = (tempx * gap_lines) + start_of_maxtrix_x;
		    }
		    else
		    {
		    	tempx = 10;
			}
			  
			if(tempy >= gap_lines)
			{
				tempy = tempy / gap_lines;
				tempy = (tempy* gap_lines) + start_of_maxtrix_y;
		    }
			else
			{
			tempy = 10;
			}	
			if(tempx < 770 && tempy < 770)
			{  
			    setfillstyle(1,15); // được sử dụng để thay đổi màu sắc và kiểu kiểu được sử dụng // 10 cho màu xanh lá cây
			    int node_index = map_points_to_nodes(tempx, tempy , gap_lines, num_columns);
			    if(node_index != start_node_index && node_index!= end_node_index)
				{
				    bar(tempx+5, tempy+5 , tempx + 15 , tempy + 15);
			        record_obstacles.insert(node_index);
			    }
			    //cout<<"Obstacle size is :"<<record_obstacles.size()<<endl;
			
			}
		} 
		else if(GetAsyncKeyState(VK_RBUTTON)) 
		{
			settextstyle(1, HORIZ_DIR,2);
	        //x coordinate, y coordinate, kiểm tra để hiển thị
            outtextxy(800, 20 + 20, "                                       ");
			outtextxy(800, 20 + 20 + 20 + 10, "                            ");
			outtextxy(800, 20 + 20 + 20 + 20 + 20, "                        ");
			outtextxy(800, 20 + 20 + 20 + 20 + 60, "                         ");
			outtextxy(800, 20 + 20 + 20 + 20 + 80, "                          ");
			outtextxy(800, 20 + 20 + 20 + 20 + 100, "                          ");
			outtextxy(800, 20 + 20, "Visualizing");
	 		outtextxy(800, 20 + 20 + 20 + 10, "Dijkstra");
	 		outtextxy(800, 20 + 20 + 20 + 20 + 20, "Algorithm");
	 		outtextxy(800,20 + 20 + 20 + 20 + 20 + 100, "Select");
			return;
		}
	 	delay(2);
	}
}
//để đánh dấu đường đi
void path_color(vector<int> path)
{
	for(int i = 0 ;i < path.size() ;i++)
	{
		nodes_to_map_points(path[i],4,10);
	}
}
//đây là phần quan trọng của thuật toán Dijkstra, hãy tìm hiểu về thuật toán Dijkstra trước khi đọc bài code này, nó sẽ giúp bạn hiểu rõ hơn !
bool dijkstra(int start, int ends, int num_of_nodes)
{ 
   	multimap<int,int> minheap;
   	vector<int> parent_track(num_of_nodes, -1);
   	map<int,int > S;
   	for(int i = 0 ; i < num_of_nodes ; i++)
   	{
   		if(i == start)
   		{
   			minheap.insert({0,start});
		}
		else
		{
			minheap.insert({INT_MAX,i});
		}
	}
	while(!minheap.empty() && minheap.begin()->first!=INT_MAX)
	{ 
	   setfillstyle(1,3);
	   bar(startx+5, starty+5 , startx + 15 , starty + 15);
	   setfillstyle(1,5);
	   bar(endx+5, endy+5 , endx + 15 , endy + 15);
	   multimap<int,int>::iterator itr = minheap.begin();
	   vector<int> adjacent_vertices = adja[itr->second];
	   int parent_weight = itr->first;
	   cout<<parent_weight<<endl;
	   int parent = itr->second;
	   S.insert({itr->second,itr->first});
	   minheap.erase(itr);
	   for(int i = 0 ; i < adjacent_vertices.size(); i++)
	   {
			multimap<int,int>::iterator itr1;
			for(itr1 = minheap.begin(); itr1!=minheap.end(); itr1++)
			{
				if(itr1->second == adjacent_vertices[i])
				break;
			}
			if(itr1 != minheap.end())
			{
				if((itr1->first) > (parent_weight + 1))
				{
					minheap.erase(itr1);
					minheap.insert({parent_weight + 1, adjacent_vertices[i]});
					parent_track[adjacent_vertices[i]] = parent;
				}
			}	
    	}
      	nodes_to_map_points(parent,14,0);
	  if(parent == ends)
	    break;
    }
    if(S.find(ends)!=S.end())
    { 
		int dist = S.find(ends)->second;
		dis = dist;
		int temp = parent_track[ends];
		vector<int> path;
		while( temp!= -1)
		{  
			path.push_back(temp);
			temp = parent_track[temp];	
		}
		path.erase(path.end()-1);
		std::reverse(path.begin(),path.end());
		path_color(path);
		return true;
  	}
  	else
	{
		return false;
	}
}

//Hàm này tạo ra ma trận trên màn hình với một khoảng trống cố định được truyền qua.
void make_graph(int screen_width, int screen_height ,int gap_lines)
{
    for(int i = 10 ; i <=screen_width ; i+=gap_lines)
	{
		line(i,10,i,screen_height);	
	}
	 
	for(int i = 10 ; i <=screen_height ; i+=gap_lines)
	{
		line(10,i,screen_width,i);	
	}	
}
//logic để tạo danh sách tính từ sử dụng danh sách kề nó vì nó là ma trận khoảng trắng ít nhất 8 nút liền kề với một nút cụ thể
//không liên quan để biểu diễn nó trong ma trận kề becuase cho mỗi nút ít nhất (n ^ 2 - 8) không gian sẽ bị lãng phí.
void make_adjacency_list(int num_of_rows,int num_of_columns)
{
	//vector<int> adja[num_of_columns * num_of_rows];
	for(int i = 0 ; i < num_of_rows; i++)
	{
		for(int j = 0 ; j < num_of_columns ; j++)
		{	
			int temp = 1;
			temp = (num_of_columns * i) + j;
			if((i-1) >= 0)
	 	   	{
				if((j-1) >= 0)
				{
					int temp2 = 1; temp2 = (num_of_columns * (i-1)) + (j - 1);
					if(record_obstacles.find(temp2) == record_obstacles.end())
					adja[temp].push_back(temp2);
				}
				if(j >= 0)
				{
					int temp2 = 1; temp2 = (num_of_columns * (i-1)) + (j);
					if(record_obstacles.find(temp2) == record_obstacles.end())
					adja[temp].push_back(temp2);
				}
				if((j + 1) < num_of_columns)
				{
					int temp2 = 1; temp2 = (num_of_columns * (i-1)) + (j + 1);
					if(record_obstacles.find(temp2) == record_obstacles.end())
					adja[temp].push_back(temp2);
				}	
	       }
	       if(i >= 0)
	       {
	    		if((j-1) >= 0)
			   	{
			    	int temp2 = 1; temp2 = (num_of_columns * (i)) + (j - 1);
			    	if(record_obstacles.find(temp2) == record_obstacles.end())
			    	adja[temp].push_back(temp2);
				}
		  		if((j + 1) < num_of_columns)
			  	{
			     	int temp2 = 1; temp2 = (num_of_columns * (i)) + (j + 1);
			    	if(record_obstacles.find(temp2) == record_obstacles.end())
			    	adja[temp].push_back(temp2);
			  	}
	    	}
			if((i+1) < num_of_rows)
			{
				if((j-1) >= 0)
				{
					int temp2 = 1; temp2 = (num_of_columns * (i+1)) + (j - 1);
					if(record_obstacles.find(temp2) == record_obstacles.end())
					adja[temp].push_back(temp2);
				}
				if(j >= 0)
				{
					int temp2 = 1; temp2 = (num_of_columns * (i+1)) + (j);
					if(record_obstacles.find(temp2) == record_obstacles.end())
					adja[temp].push_back(temp2);
				} 
				if((j + 1) < num_of_columns)
				{
					int temp2 = 1; temp2 = (num_of_columns * (i+1)) + (j + 1);
					if(record_obstacles.find(temp2) == record_obstacles.end())
					adja[temp].push_back(temp2);
				}
			}
		}
	}
}	
//nó chuyển đổi khoảng cách được tính toán thành chuỗi để có thể hiển thị trên màn hình
string distance_to_string(int distance)
{
	string temp2= "";
	while(distance!=0)
	{
		int temp = distance % 10;
		temp+=48;
		temp2+=temp;
		distance = distance/10;
	}
	if(temp2.size() > 1)
	reverse(temp2.begin(), temp2.end());
	cout<<"Distance is: "<<temp2<<" Size is "<<temp2.length()<<endl;
	return temp2;
}
int main()
{
	DWORD screenWidth = GetSystemMetrics(SM_CXSCREEN);
	DWORD screenHeight = GetSystemMetrics(SM_CYSCREEN);
	int start_of_maxtrix_x = 10;
	int start_of_maxtrix_y = 10;
    initwindow(1080,850,"test");	 
	int num_lines_vertical; // this uses screen width
	int num_lines_horizontal; //this uses screen height
	int gap_lines = 20;
	int screen_width = 770;
	int screen_height = 770;
	int num_rows = 38;
	int num_columns = 38;
	int start_node_index = 0;
	int end_node_index = 0;
	int num_of_nodes = 38*38;
  	make_graph(screen_width,screen_height,gap_lines); 
	while(1)	  
	{
		make_graph(screen_width,screen_height,gap_lines); 
		POINT cursorPosition;
		int mx = 0,my = 0;
		int count = 0;
		map<int,int> mymap;
		settextstyle(1, HORIZ_DIR,2);
		//dùng để làm đẹp màn hình chính cho nút bắt đầu
		setfillstyle(1,3); //được sử dụng để thay đổi màu sắc và kiểu được sử dụng
		bar(780+5, 450+5 , 780 + 15 , 450 + 15);
		outtextxy(800, 450, "Starting Node");
		setfillstyle(1,5); //for ending node
		bar(780+5, 490+5 , 780 + 15 , 490 + 15);
		outtextxy(800, 490, "Ending Node");
		setfillstyle(1,14); //for relaxed nodes
		bar(780+5, 530+5 , 780 + 15 , 530 + 15);
		outtextxy(800, 530, "Relaxed Node"); 
		setfillstyle(1,4); //for path Nodes
		bar(780+5, 570+5 , 780 + 15 , 570 + 15);
		outtextxy(800, 570, "Path Node");  
		setfillstyle(1,15); //for obstacle Nodes
		bar(780+5, 610+5 , 780 + 15 , 610 + 15);
		outtextxy(800, 610, "Obstacle Node");  
		while(1)
		{
			//phông chữ, hướng, kích thước ký tự
			settextstyle(1, HORIZ_DIR,2);
			//x coordinate, y coordinate, kiểm tra để hiển thị
			outtextxy(800, 20 + 20, "Visualizing");
			outtextxy(800, 20 + 20 + 20 + 10, "Dijkstra");
			outtextxy(800, 20 + 20 + 20 + 20 + 20, "Algorithm");
			outtextxy(800,20 + 20 + 20 + 20 + 20 + 100, "Select");
			if(mymap.size() == 0)
			outtextxy(800, 20 + 20 + 20 + 20 + 20 + 120, "The Starting Node");
			GetCursorPos(&cursorPosition);
				mx = cursorPosition.x - 10; //to overcome the difference
				my = cursorPosition.y - 30;  //to overcome the difference
			//cout<<mx<<" "<<my<<endl;
			if(GetAsyncKeyState(VK_LBUTTON))
			{
				int tempx = mx - start_of_maxtrix_x ;
				int tempy = my - start_of_maxtrix_y ;
				
				if(tempx >= gap_lines)
				{
				tempx = tempx / gap_lines;
				tempx = (tempx * gap_lines) + start_of_maxtrix_x;
				}
				else
				{
					tempx = 10;
				}
				if(tempy >= gap_lines)
				{
				tempy = tempy / gap_lines;
				tempy = (tempy* gap_lines) + start_of_maxtrix_y;
				}
				else
				{
					tempy = 10;
				}	
				mymap.insert({mx,my});
				if(mymap.size() == 1)
				{  
					setfillstyle(1,3); //used to change the color and type of style used
					bar(tempx+5, tempy+5 , tempx + 15 , tempy + 15);
					startx = tempx;
					starty = tempy;
					// cout<<tempx<<" "<<tempy<<endl;
					start_node_index = map_points_to_nodes(tempx, tempy , gap_lines, num_columns);
					outtextxy(800, 20 + 20 + 20 + 20 + 20 + 120, "The Ending  Node");   
				}
				if(mymap.size() == 2)
				{  	setfillstyle(1,5);
					bar(tempx+5, tempy+5 , tempx + 15 , tempy + 15);
					outtextxy(800,20 + 20 + 20 + 20 + 20 + 100, "                ");
					outtextxy(800, 20 + 20 + 20 + 20 + 20 + 120, "                            ");
					end_node_index = map_points_to_nodes(tempx, tempy, gap_lines, num_columns);
					//cout<<tempx<<" "<<tempy;
					endx = tempx;
					endy = tempy;
				}
				if(mymap.size() == 2)
				break;
			} 
			delay(10);
		}
		if(start_node_index != end_node_index)
		{
			outtextxy(800, 20 + 20 + 20 + 20 + 20 + 140, "Obstacles? (Y/N)");
			outtextxy(800, 20 + 20 + 20 + 20 + 20 + 160, "Input Compulsary");
			while(1)
			{
				char c;
				c  = (char) getch();
				if( c == 121 || c == 89) //ascii value of 'y' of 'Y'
				{
					add_obstacles(start_node_index,end_node_index);
					outtextxy(800, 20 + 20 + 20 + 20 + 20 + 140, "                                   ");
					outtextxy(800, 20 + 20 + 20 + 20 + 20 + 160, "                                   ");
					break;
				}
				if( c == 110 || c == 78) //ascii value of 'n' of 'N'
				{
					outtextxy(800, 20 + 20 + 20 + 20 + 20 + 140, "                                   ");
					outtextxy(800, 20 + 20 + 20 + 20 + 20 + 160, "                                   ");
					break;	
				}
				else
				{
					//nhấn phím sai, hãy thử lại                    
					outtextxy(780, 20 + 20 + 20 + 20 + 20 + 120 + 60, "Wrong Key Pressed   ");
					outtextxy(780, 20 + 20 + 20 + 20 + 20 + 120 + 90, "Press Y to try again");
					outtextxy(780, 20 + 20 + 20 + 20 + 20 + 120 + 120, "Press N to exit");													
				}
			}
			//tạo danh sách adjacency_List mới sau mỗi chướng ngại vật được ghi lại
			make_adjacency_list(38,38);	 
			cout<<"Start Node Index " << start_node_index <<" End Node Index "<<end_node_index;
			//cdoe dijkstra bắt đầu ở đây
			if(dijkstra(start_node_index, end_node_index,num_of_nodes))
			{
			cout<<"\n Hello World";
			outtextxy(800,20 + 20 + 20 + 20 + 20 + 100, "          ");
			string s = "The Distance is: ";
			
			// cout<<s.length()<<endl;
			s+=distance_to_string(dis);
			s+= "  ";
		
			int n = s.length(); 
			//cout<<n<<endl;
			// declaring character array 
			char char_array[n]; 
		
			// copying the contents of the 
			// string to char array 
			strcpy(char_array, s.c_str()); 
			outtextxy(780, 20 + 20 + 20 + 20 + 20 + 120, char_array);
			}
			else
			{
				outtextxy(780, 20 + 20 + 20 + 20 + 20 + 120, "No Path Found Retry");
			}
		}
		else
		{
			outtextxy(780, 20 + 20 + 20 + 20 + 20 + 140, "Start and End Same");
			outtextxy(780, 20 + 20 + 20 + 20 + 20 + 160, "Distance is : 0");
		}
		outtextxy(780, 20 + 20 + 20 + 20 + 20 + 120 + 60, "Press R to try again");
		outtextxy(780, 20 + 20 + 20 + 20 + 20 + 120 + 90, "Press Esc to exit");
		while(1)
		{
			char c;
			c  = (char) getch();
			if(c == 27) //27 is the ascii for "esc" 
			{
				exit(1);
			}
			if( c == 114 || c == 82) //ascii value of 'r' of 'R'
			break;
			else
			{
				//nhập sai xin hãy thử lại                    
				outtextxy(780, 20 + 20 + 20 + 20 + 20 + 120 + 60, "Wrong Key Pressed   ");
				outtextxy(780, 20 + 20 + 20 + 20 + 20 + 120 + 90, "Press R to try again");
				outtextxy(780, 20 + 20 + 20 + 20 + 20 + 120 + 120, "Press Esc to exit");                                                      
			}
		}
		//xóa danh sách gần kề cũng như các chướng ngại vật được ghi lại cho lần lặp tiếp theo được yêu cầu
		record_obstacles.clear();
		for(int i = 0 ; i < num_of_nodes ; i++)
			adja[i].clear();
			cleardevice();
	}
	getch();
	closegraph();
	return 0;
}