#include"graph.h"

//constructor to initalize bool table
graph::graph()
{
  size = MAX; //make size MAX (20)
  copy = new bool[size]; //make a bool chart
  for(int i = 0; i < size; i++) //initialize the elements to false
    {
      copy[i] = false;
    }
}

//constructor to initalize edge and vertex tables
graph::graph(int value)
{
  size = value; //value is size, which is MAX
  copy = new bool[size]; //bool table
  for(int i = 0; i < size; i++)
    {
      copy[i] = false;
    }
  array = new int*[size]; //edge table c
  for(int i = 0; i < size; i++)
    {
      array[i] = new int[size]; //make space for a 2d array
      for(int k = 0; k < size; k++)
	{
	  array[i][k] = 0; //initialize index values to 0
	}
    }
  verts = new char*[size]; //make a vertex table inserting into the array table (creating a 2 dim. array)
}

//deconstructor to delete everything
graph::~graph()
{
  for(int i = 0; i < size; i++)
    {
      delete[] array[i]; //delete each index in the array
    }
  delete[] array; //delete the entire array itself
  for(int i = 0; i < size; i++)
    {
      if(copy[i] == true) //if not already deleted (use of bool table)
	{
	  delete[] verts[i]; //then delete the index
	}
    }
  delete[] verts; //delete the entire verts table
  delete[] copy; //delete the bool table after
  size = 0; //make size = 0;
}

//Add Vertex
int graph::addV(const char* label)
{
  int index = findIndex(label); //find the index for the label
  if(index == -1) //if it returns -1 (-1 means "empty/space allocated")
    {
      return 0; //do nothing
    }
  if(index < size) //if the index is less than the size
    {
      verts[index] = new char[strlen(label)+1]; //make a new vertex in the next empty spot
      strcpy(verts[index], label); //copy label into the vertex[index]
      copy[index] = true; //make the bool value true to indicate that it is an active, existing vertex
      return 1;
    }
  else //if size is smaller (does not fit in array)
    {
      return 0; //do nothing
    }
}

//Display the vertices and edge values between them
int graph::display()
{
  cout << " ";
  for(int i = 0 ; i < size; i++)
    {
      if(copy[i] == true)
	{
	  cout << " " << verts[i];
	}
    }
  for(int i = 0; i < size; i++)
    {
      if(copy[i] == true)
	{
	  cout << endl << verts[i];
	  for(int k = 0; k < size; k++)
	    {
	      if(copy[k] == true)
		{
		  cout << " " << array[i][k];
		}
	    }
	}
    }
  /*
  for(int i = 0; i < size; i++)
    {
      if(copy[i] == true)
	{
	  for(int k = 0; k < size; k++)
	    {
	      if(copy[k] == true)
		{
		  cout << " " << array[i][k];
		}
	    }
	  cout << endl;
	}
	}*/
  return 1;
}

int graph::remV(const char* label)
{
  int index = findIndex(label);
  if(index == -1)
    {
      return 0;
    }
  if(index < size)
    {
      for(int i = 0; i < size; i++)
	{
	  array[index][i] = 0;
	  array[i][index] = 0;
	  }
      delete[] verts[index];
      copy[index] = false;
      return 1;
    }
  else
    {
      return 0;
    }
}


int graph::addE(const char* l1, const char* l2, int weight)
{
  int index1 = findIndex(l1);
  int index2 = findIndex(l2);
  
  if(index1 == -1 || index2 == -1) //if both indexes are empty (-1)
    {
      return 0;
    }
  array[index1][index2] = weight;
  return 1;
}

int graph::remE(const char* l1, const char* l2)
{
  int index1 = findIndex(l1);
  int index2 = findIndex(l2);
  if(index1 == -1 || index2 == -1)
    {
      return 0;
    }
  else
    {
      array[index1][index2] = 0;
      return 1;
    }
}


int graph::findIndex(const char* label)
{
  int index = 0;
  while(copy[index] == true && strcmp(verts[index], label) != 0 && (index < size))
    {
      index++;
    }
  if(index < size)
    {
      return index;
    }
  else
    {
      return -1;
    }
}

int graph::findPath(const char* l1, const char* l2)
{
  int index1 = findIndex(l1);
  int index2 = findIndex(l2);
  if(index1 == -1 || index2 == -1)
    {
      return 0; //no edges
    }
  bool visit[size];
  int dist[size];
  int index[size];

  for(int i = 0; i < size; i++)
    {
      visit[i] = false;
      dist[i] = INT_MAX;
      index[i] = -1;
    }

  dist[index1] = 0;

  for(int i = 0; i < size - 1; i++)
    {
      int shortEdge = shortE(visit, dist);

      visit[shortEdge] = true;
      for(int k = 0; k < size; k++)
	{
	  if(!visit[k] && array[shortEdge][k] && dist[shortEdge] != INT_MAX && ((array[shortEdge][k] + dist[shortEdge]) < dist[k]))
	    {
	      index[k] = shortEdge;
	      dist[k] = array[shortEdge][k] + dist[shortEdge];
	    }
	}
    }
  for(int i = 0; i < size; i++)
    {
      if(copy[i] == true)
      cout << visit[i] << " ";
    }
  cout << endl;
  for(int i = 0; i < size; i++)
    {
      if(copy[i] == true)
      cout << dist[i] << " ";
    }
  cout << endl;
  for(int i = 0; i < size; i++)
    {
      if(copy[i] == true)
      cout << index[i] << " ";
    }
  
  
  cout << verts[index1] << " ";
  printPath(index, index2);
  cout << " -> ";
  cout << "\nWeight: " << dist[index2];
}

int graph::shortE(bool visit[], int weight[])
{
  int min = INT_MAX;
  int index;

  for(int i = 0; i < size; i++)
    {
      if(visit[i] == false && weight[i] < min)
	{
	  min = weight[i];
	  index = i;
	}
    }
  return index;
}

void graph::printPath(int weight[], int index)
{
  if(weight[index] == -1)
    {
      return;
    }
  else
    {
      printPath(weight, weight[index]);
      cout << verts[index] << " -> ";
    }
}
