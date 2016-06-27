#include "LeapData.h"
#include <sstream>


LeapData::LeapData() {
	//default constructor
}

// constructor with a path as a parameter
LeapData::LeapData(std::string path) {
	std::ifstream ifs;
	std::string line;

	ifs.open(path, std::ifstream::in);

	if (ifs.is_open()) {
		int counter = 0;
		while (!ifs.eof()) {
			std::getline(ifs, line);
			parse(counter, line);
			counter++;
		}
		setNewScaleFactor();
		setOldScaleFactor();
		setNewFingertipDist();
		projectionPoints = getProjection();
		setFingertipAngles();
		setRatio();
		


		//gets the gesture based on the path
		int index = path.find_last_of("/");
		char gestureNumber = path.at(index - 1);
		gesture = "G";
		gesture.push_back(gestureNumber);
		if (gesture == "Gd") {
			std::cout << path << std::endl;
		}

		if (gesture == "G0") {
			extendedFingers = { 0,1,1,0,1 };
		}
		else if (gesture == "G1") {
			extendedFingers = { 0,0,0,0,0 };
		}
		else if (gesture == "G2") {
			extendedFingers = { 0,1,0,0,0 };
		}
		else if (gesture == "G3") {
			extendedFingers = { 0,0,0,0,1 };
		}
		else if (gesture == "G4") {
			extendedFingers = { 1,1,0,0,0 };
		}
		else if (gesture == "G5") {
			extendedFingers = { 0,1,1,0,0 };
		}
		else if (gesture == "G6") {
			extendedFingers = { 0,1,1,1,0 };
		}
		else if (gesture == "G7") {
			extendedFingers = { 1,0,0,0,1 };
		}
		else if (gesture == "G8") {
			extendedFingers = { 1,1,0,0,1 };
		}
		else if (gesture == "G9") {
			extendedFingers = { 1,1,1,1,1 };
		}

		setFingerAreas();
	}
	else {
		std::cout << "bad file" << std::endl;
	}
	ifs.close();
}

// constructor with RealTimeLeapData object as parameter
LeapData::LeapData(RealTimeLeapData leapData) {
	extendedFingers = leapData.getExtendedFingers();
	fingerDirections = leapData.getFingerDirections();
	fingertipPosition = leapData.getTipPositions();
	handDirection = leapData.getHandDirection();
	palmNormal = leapData.getPalmNormal();
	palmPosition = leapData.getPalmPosition();
	numFingers = leapData.getNumFingers();
	setNewScaleFactor();
	setNewFingertipDist();
	projectionPoints = getProjection();
	setFingertipAngles();
	gesture = leapData.getGesture();
	setFingerAreas();
	setRatio();
}

// sets fingerArea to the average area between the extended fingers
// s = (fingerDistance[i] + fingerDistance[i+1] + distanceBetweenFingers) / 2
// area = sqrt(s*(s - fingerDistance[i])*(s - fingerDistance[i+1])*(s - distanceBetweenFingers)) / newScaleFactor
// fingerArea = area / numFingers
void LeapData::setFingerAreas()
{
	float total;
	for (int i = 0; i < 4; i++) {
		if((i+1)<numFingers)
		{
			float a = fingertipPosition[i].getMagnitude(palmPosition);
			float b = fingertipPosition[i].getMagnitude(fingertipPosition[i + 1]);
			float c = fingertipPosition[i+1].getMagnitude(palmPosition);
			float s = (a + b + c) / 2;
			float area = sqrt(s*(s - a)*(s - b)*(s - c))/newScaleFactor;
			//total = total + area;
			if (area > total) {
				total = area;
			}

		}
		
	}
	if (numFingers >= 2) {
		fingerArea = total / (numFingers - 1);
	}
	else {
		fingerArea = 0.000000;
	}
}
void LeapData::setRatio()
{
	//fingertipDistRefined
	float maxX = 0;
	float maxY = 0;
	int leftFingerAddr;
	int rightFingerAddr;
	Point right;
	Point left;
	float tmp;
	for (int i = 0; i < fingertipPosition.size(); i++)
		{
			if ((fingertipPosition[i].getX() != 0) && (fingertipPosition[i].getY() != 0) && (fingertipPosition[i].getZ() != 0))
			{
				tmp = fingertipPosition[i].getY()-palmPosition.getY() ;
			}
			if (tmp < 0)
			{
				tmp = tmp*-1;
			}
			
			if (tmp > maxY)
			{
				maxY = tmp;
			}
		}
	
	for (int i = 0; i < fingertipPosition.size(); i++)
	{
		if ((fingertipPosition[i].getX() != 0)&& (fingertipPosition[i].getY() != 0)&& (fingertipPosition[i].getZ() != 0))
		{
			leftFingerAddr = i;
			left = fingertipPosition[leftFingerAddr];
			break;
		}
	}
	for (int i=fingertipPosition.size()-1; i >= 0; i--)
	{
		if ((fingertipPosition[i].getX() != 0) && (fingertipPosition[i].getY() != 0) && (fingertipPosition[i].getZ() != 0))
		{
			rightFingerAddr = i;
			right = fingertipPosition[rightFingerAddr];
			break;
		}
	}
	
/*	std::cout << "rightFingerAddr: " << rightFingerAddr<<std::endl;
	std::cout << "right finger: "; right.printPoint();
	
	std::cout << "leftFingerAddr: " << leftFingerAddr << std::endl;
	std::cout << "left finger: "; left.printPoint();

	std::cout << "fingertipPosition:" << " ";
	for (int i = 0; i < fingertipPosition.size(); i++) {
		fingertipPosition[i].printPoint();
	}
	std::cout << std::endl;
	
	std::cout << "extended Finger:" << " ";
	for (int i = 0; i < extendedFingers.size(); i++) {
		std::cout << extendedFingers[i];
	}
	std::cout << std::endl;*/

	maxX = right.getX() - left.getX();
	//maxX = right.getMagnitude(left);
	
	ratio = maxY / maxX;

	if (ratio < 0)
	{
		ratio = ratio*-1;
	}
	if (numFingers == 1)
	{
		ratio = maxY;
	}
	if (numFingers == 0)
	{
		ratio = 0;
	}
	/*std::cout << 
		"maxX: " << maxX << "\n" << 
		"maxY: " << maxY << "\n" << 
		"ratio: " << ratio<<"\n";*/

}
// sets the fingertipAngles and also fingertipElevation
// fingertipElevation = ||fingertipPosition - projectedPoint|| / newScaleFactor
// fingertipAngles are determined using law of cosines
void LeapData::setFingertipAngles() {
	float a, b, c;
	for (int i = 0; i < 5; i++) {
		if (i >= numFingers) {
			fingertipAngles.push_back(0);
			fingertipElevation.push_back(0);
		}
		else {
			a = palmPosition.getMagnitude(fingertipPosition[i]);;
			b = projectionPoints[i].getMagnitude(palmPosition);
			c = projectionPoints[i].getMagnitude(fingertipPosition[i]);
			fingertipAngles.push_back(acos((-powf(c, 2) + powf(a, 2) + powf(b, 2)) / (2 * a * b)));
			fingertipElevation.push_back(c / newScaleFactor);
		}
	}
}

// sets the newFingertipDistance based on newScaleFactor
// newFingertipDist = dist(fingertipPosition, palmPosition) / newScaleFactor
void LeapData::setNewFingertipDist() {
	for (int i = 0; i < 5; i++) {
		if (i >= numFingers) {
			newFingertipDistRefined.push_back(0.000000);
		}
		else {
			newFingertipDistRefined.push_back(fingertipPosition[i].getMagnitude(palmPosition) / newScaleFactor);
		}
	}
}

// calculates newScaleFactor
// newScaleFactor = dist((avg(fingertipPositions) / numFingers), palmPosition)
void LeapData::setNewScaleFactor() {
	if (numFingers == 0) {
		newScaleFactor = 1;
	}
	else {
		float averageX = 0;
		float averageY = 0;
		float averageZ = 0;
		for (int i = 0; i < numFingers; i++) {
			averageX = averageX + fingertipPosition[i].getX();
			averageY = averageY + fingertipPosition[i].getY();
			averageZ = averageZ + fingertipPosition[i].getZ();
		}
		Point p = Point(averageX / numFingers, averageY / numFingers, averageZ / numFingers);
		newScaleFactor = p.getMagnitude(palmPosition);

	}
}

// scale factor based on longest finger
void LeapData::setOldScaleFactor() {
	if (numFingers == 0) {
		oldScaleFactor = 1;
	}
	else {
		oldScaleFactor = 0;
		for (int i = 0; i < 5; ++i)
		{
			if (fingertipDist[i] > oldScaleFactor)
			{
				oldScaleFactor = fingertipDist[i];
			}
		}
	}
}

// returns vector of Points projected into normal plane
// projection = q(point to project) - dot(handDirection, distance between Point and plane)
std::vector<Point> LeapData::getProjection() {
	std::vector<Point> returnVal;
	// p = (palmNormal / |palmnormal|) * 1
	float k = 1 / palmPosition.getMagnitude(palmNormal);
	Point p = Point(palmNormal.getX() * k, palmNormal.getY() * k, palmNormal.getZ() * k);
	//d = fingertipPosition - p
	// returnVal = fingertipPosition - dot(handDirection, d) * handDirection
	for (int i = 0; i < 5; i++) {
		if (i >= numFingers) {
			returnVal.push_back(Point(0,0,0));
		}
		else {
			Point d = Point(fingertipPosition[i].getX() - p.getX(), fingertipPosition[i].getY() - p.getY(), fingertipPosition[i].getZ() - p.getZ());
			returnVal.push_back(Point(fingertipPosition[i].getX() - handDirection.getDotProduct(d) * handDirection.getX(),
				fingertipPosition[i].getY() - handDirection.getDotProduct(d) * handDirection.getY(),
				fingertipPosition[i].getZ() - handDirection.getDotProduct(d) * handDirection.getZ()));
		}
	}
	return returnVal;
}

// parse all the attributes from CSV file
void LeapData::parse(int lineNum, std::string line) {
	std::vector<float> vect = splitString(line);
	std::vector<Point> pointVect;
	switch (lineNum) {
	case 0:
		numFingers = vect[0];
		break;
	case 1:
		fingertipDist = vect;
		break;
	case 2:
		fingertipDistRefined = vect;
		break;
	case 3:
		fingertipInterDist = vect;
		break;
	case 4:
		for (int i = 0; i < 5; i++) {
			pointVect.push_back(Point(vect[i], vect[i+5], vect[i+10]));
		}
		fingertipPosition = pointVect;
		break;
	case 5:
		handDirection = Point(vect[0], vect[1], vect[2]);
		break;
	case 6:
		handSphereCenter = Point(vect[0], vect[1], vect[2]);
		break;
	case 7:
		handSphereRadius = vect[0];
		break;
	case 8:
		palmNormal = Point(vect[0], vect[1], vect[2]);
		break;
	case 9:
		palmPosition = Point(vect[0], vect[1], vect[2]);
		break;
	case 10:
		palmPositionRefined = Point(vect[0], vect[1], vect[2]);
		break;
	case 11:
		palmVelocity = vect;
		break;
	case 12:
		rotationAngle = vect[0];
		break;
	case 13:
		rotationAxis = vect;
		break;
	case 14:
		rotationMatrix = vect;
		break;
	case 15:
		rotationProbability = vect[0];
		break;
	case 16:
		scaleFactor = vect[0];
		break;
	case 17:
		scaleProbability = vect[0];
		break;
	case 18:
		translation = vect;
		break;
	case 19:
		translationProbability = vect[0];
		break;
	default:
		break;
	}

}

// returns vector of floats with CSV line content split at the commas
std::vector<float> LeapData::splitString(std::string line) {
	std::istringstream ss(line);
	std::string token;
	int counter = 0;
	std::vector<float> returnVal;
	std::string::size_type sz;

	while (std::getline(ss, token, ',')) {
		if (counter != 0) {
			returnVal.push_back(std::stof(token, &sz));
		}
		counter++;
	}

	return returnVal;
}

// prints all the attributes
void LeapData::printAttributes() {
	std::cout << "number of fingers: " << numFingers << std::endl;

	std::cout << "fingertipDist:" << " ";
	for (int i = 0; i < fingertipDist.size(); i++) {
		std::cout << fingertipDist[i] << " ";
	}
	std::cout << std::endl;

	std::cout << "fingertipDistRefined:" << " ";
	for (int i = 0; i < fingertipDistRefined.size(); i++) {
		std::cout << fingertipDistRefined[i] << " ";
	}
	std::cout << std::endl;

	std::cout << "fingertipInterDist:" << " ";
	for (int i = 0; i < fingertipInterDist.size(); i++) {
		std::cout << fingertipInterDist[i] << " ";
	}
	std::cout << std::endl;

	std::cout << "handDirection:" << " ";
	handDirection.printPoint();
	std::cout << std::endl;

	std::cout << "palmNormal:" << " ";
	palmNormal.printPoint();
	std::cout << std::endl;

	std::cout << "palmVelocity:" << " ";
	for (int i = 0; i < palmVelocity.size(); i++) {
		std::cout << palmVelocity[i] << " ";
	}
	std::cout << std::endl;

	std::cout << "rotationAxis:" << " ";
	for (int i = 0; i < rotationAxis.size(); i++) {
		std::cout << rotationAxis[i] << " ";
	}
	std::cout << std::endl;

	std::cout << "rotationMatrix:" << " ";
	for (int i = 0; i < rotationMatrix.size(); i++) {
		std::cout << rotationMatrix[i] << " ";
	}
	std::cout << std::endl;

	std::cout << "translation:" << " ";
	for (int i = 0; i < translation.size(); i++) {
		std::cout << translation[i] << " ";
	}
	std::cout << std::endl;

	std::cout << "handSphereRadius: " << handSphereRadius << std::endl;

	std::cout << "rotationAngle: " << rotationAngle << std::endl;

	std::cout << "rotationProbability: " << rotationProbability << std::endl;

	std::cout << "scaleFactor: " << scaleFactor << std::endl;

	std::cout << "scaleProbability: " << scaleProbability << std::endl;

	std::cout << "translationProbability: " << translationProbability << std::endl;

	std::cout << "fingertipPosition:" << " ";
	for (int i = 0; i < fingertipPosition.size(); i++) {
		fingertipPosition[i].printPoint();
	}
	std::cout << std::endl;

	std::cout << "handSphereCenter:" << " ";
	handSphereCenter.printPoint();
	std::cout << std::endl;

	std::cout << "palmPosition:" << " ";
	palmPosition.printPoint();
	std::cout << std::endl;

	std::cout << "palmPositionRefined:" << " ";
	palmPositionRefined.printPoint();
	std::cout << std::endl;

	std::cout << "New scale factor: " << newScaleFactor << std::endl;
	std::cout << "Old scale factor: " << oldScaleFactor << std::endl;

	std::cout << "newFingertipDistRefined:" << " ";
	for (int i = 0; i < newFingertipDistRefined.size(); i++) {
		std::cout << newFingertipDistRefined[i] << " ";
	}
	std::cout << std::endl;

	std::cout << "fingertipAngles:" << " ";
	for (int i = 0; i < fingertipAngles.size(); i++) {
		std::cout << fingertipAngles[i] << " ";
	}
	std::cout << std::endl;

	std::cout << "projectionPoints:" << " ";
	for (int i = 0; i < projectionPoints.size(); i++) {
		projectionPoints[i].printPoint();
	}
	std::cout << std::endl;

	std::cout << "fingertipElevation:" << " ";
	for (int i = 0; i < fingertipElevation.size(); i++) {
		std::cout << fingertipElevation[i] << " ";
	}
	std::cout << std::endl;
}

int LeapData::getNumFingers() {
	return numFingers;
}
std::vector<float> LeapData::getFingertipDist() {
	return fingertipDist;
}
std::vector<float> LeapData::getFingertipDistRefined() {
	return fingertipDistRefined;
}
std::vector<float> LeapData::getFingertipInterDist() {
	return fingertipInterDist;
}
std::vector<Point> LeapData::getFingertipPosition() {
	return fingertipPosition;
}
Point LeapData::getHandDirection() {
	return handDirection;
}
Point LeapData::getHandSphereCenter() {
	return handSphereCenter;
}
float LeapData::getHandSphereRadius() {
	return handSphereRadius;
}
Point LeapData::getPalmNormal() {
	return palmNormal;
}
Point LeapData::getPalmPosition() {
	return palmPosition;
}
Point LeapData::getPalmPositionRefined() {
	return palmPositionRefined;
}
std::vector<float> LeapData::getPalmVelocity() {
	return palmVelocity;
}
float LeapData::getRotationAngle() {
	return rotationAngle;
}
std::vector<float> LeapData::getRotationAxis() {
	return rotationAxis;
}
std::vector<float> LeapData::getRotationMatrix() {
	return rotationMatrix;
}
float LeapData::getRotationProbability() {
	return rotationProbability;
}
float LeapData::getScaleFactor() {
	return scaleFactor;
}

float LeapData::getScaleProbability() {
	return scaleProbability;
}
std::vector<float> LeapData::getTranslation() {
	return translation;
}
float LeapData::getTranslationProbability() {
	return translationProbability;
}
float LeapData::getNewScaleFactor() {
	return newScaleFactor;
}
std::vector<float> LeapData::getNewFingertipDistRefined() {
	return newFingertipDistRefined;
}
std::vector<float> LeapData::getFingertipAngles() {
	return fingertipAngles;
}
std::vector<Point> LeapData::getProjectionPoints() {
	return projectionPoints;
}
std::vector<float> LeapData::getFingertipElevation() {
	return fingertipElevation;
}

std::string LeapData::getGesture() {
	return gesture;
}

std::vector<int> LeapData::getExtendedFingers() {
	return extendedFingers;
}
float LeapData::getFingerArea() {
	return fingerArea;
}
float LeapData::getRatio() {
	return ratio;
}