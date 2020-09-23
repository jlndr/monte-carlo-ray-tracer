#include "glm/vec3.hpp"
#include "glm/glm.hpp"
#include "Triangle.hpp"

#include <array>
#include <vector>

#include <iostream>
class Scene {

public:
	Scene(){};
	void drawRoom();
	
private:
	std::vector<Triangle> room;
};

void Scene::drawRoom() {

	//VERTEXES

	//
	//		p3-----p5
	// p1				 p6
	//		p2-----p4
	//
	glm::vec3 p1_up{-3.0f, 0.0f, 5.0f}, p1_down{-3.0f, 0.0f, -5.0f};
	glm::vec3 p2_up{0.0f, -6.0f, 5.0f}, p2_down{0.0f, -6.0f, -5.0f};
	glm::vec3 p3_up{0.0f, 6.0f, 5.0f}, p3_down{0.0f, 6.0f, -5.0f};
	glm::vec3 p4_up{10.0f, -6.0f, 5.0f}, p4_down{10.0f, -6.0f, -5.0f};
	glm::vec3 p5_up{10.0f, 6.0f, 5.0f}, p5_down{10.0f, 6.0f, -5.0f};
	glm::vec3 p6_up{13.0f, 0.0f, 5.0f}, p6_down{13.0f, 0.0f, -5.0f};

	//Colors
	glm::vec3 White{1.0f, 1.0f, 1.0f};
	glm::vec3 Red{1.0f, 0.0f, 0.0f};
	glm::vec3 Green{0.0f, 1.0f, 0.0f};
	glm::vec3 Blue{0.0f, 0.0f, 1.0f};
	glm::vec3 Yellow{1.0f, 1.0f, 0.0f};
	glm::vec3 Teal{0.0f, 0.5f, 0.5f};
	glm::vec3 Purple{1.0f, 0.0f, 1.0f};

	//ROOF
	//((-3, 0, 5))
	//(0, 6 , 5)
	//(0, -6, 5)
	room.push_back(Triangle{p1_up, p3_up, p2_up, White});

	//(0, -6, 5)
	//(0, 6, 5)
	//(10, 6, 5)
	room.push_back(Triangle{p2_up, p3_up, p5_up, White});	
	
	//(0, -6, 5)
	//(10, 6, 5)
	//(10, -6, 5)
	room.push_back(Triangle{p2_up, p5_up, p4_up, White});

	//(10, 6, 5)
	//(13, 0, 5)
	//(10, -6, 5)		
	room.push_back(Triangle{p5_up, p6_up, p4_up, White});

	//FLOOR
	//((-3, 0, -5))
	//(0, -6 , -5)
	//(0, 6, -5)
	room.push_back(Triangle{p1_down, p2_down, p3_down, White});
	
	//(0, -6, -5)
	//(10, 6, -5)
	//(0, 6, -5)
	room.push_back(Triangle{p2_down, p5_down, p3_down, White});

	//(0, -6, -5)
	//(10, -6, -5)
	//(10, 6, -5)
	room.push_back(Triangle{p2_down, p4_down, p5_down, White});

	//(10, -6, -5)
	//(13, 0, -5)
	//(10, 6 , -5)
	room.push_back(Triangle{p4_down, p6_down, p5_down, White});

	//Negativ xy 1
	//(-3, 0, 5)
	//(0, -6, -5)
	//(-3, 0, -5)
	room.push_back(Triangle{p1_up, p2_down, p1_down, Red});

	//Negativ xy 2
	//(-3, 0, 5)
	//(0, -6, 5)
	//(0, -6, -5)
	room.push_back(Triangle{p1_up, p2_up, p2_down, Red});


	//Negativ x pos y 1
	//(-3, 0, 5)
	//(-3, 0, -5)
	//(0, 6, -5)
	room.push_back(Triangle{p1_up, p1_down, p3_down, Green});

	//Negativ x pos y 2
	//(-3, 0, 5)
	//(0, 6, -5)
	//(0, 6, 5)
	room.push_back(Triangle{p1_up, p3_down, p3_up, Green});

	//Långsida neg Y
	//(10, -6, 5)
	//(0, -6, -5)
	//(0, -6, 5)
	room.push_back(Triangle{p4_up, p2_down, p2_up, Blue});

	//(10, -6, 5)
	//(10, -6, -5)
	//(0, -6, -5)
	room.push_back(Triangle{p4_up, p4_down, p2_down, Blue});

	//Långsida neg Y
	//(10, 6, 5)
	//(0, 6, 5)
	//(0, 6, -5)
	room.push_back(Triangle{p5_up, p3_up, p3_down, Yellow});

	//(10, 6, 5)
	//(0, 6, -5)
	//(10, 6, -5)
	room.push_back(Triangle{p5_up, p3_down, p5_down, Yellow});

	//Pos x neg y 1
	//(13, 0, 5)
	//(13, 0, -5)
	//(10, -6, -5)
	room.push_back(Triangle{p6_up, p6_down, p4_down, Purple});

	//Pos x neg y 2
	//(13, 0, 5)
	//(10, -6, -5)
	//(10, -6, 5)
	room.push_back(Triangle{p6_up, p4_down, p4_up, Purple});

	//pos x pos y 1
	//(13, 0, 5)
	//(10, 6, -5)
	//(13, 0, -5)
	room.push_back(Triangle{p6_up, p5_down, p6_down, Teal});

	//pos x pos y 2
	//(13, 0, 5)
	//(10, 6, 5)
	//(10, 6, -5)
	room.push_back(Triangle{p6_up, p5_up, p5_down, Teal});
