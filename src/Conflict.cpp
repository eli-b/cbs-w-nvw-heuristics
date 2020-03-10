#include "Conflict.h"
#include "RectangleReasoning.h"
#include "MDD.h"


std::ostream& operator<<(std::ostream& os, const Constraint& constraint)
{
	os << "<" << std::get<0>(constraint) << "," << std::get<1>(constraint) << "," <<
		std::get<2>(constraint) << "," << std::get<3>(constraint) << ",";
	switch (get<4>(constraint))
	{
		case constraint_type::VERTEX:
			os << "V";
			break;
		case constraint_type::POSITIVE_VERTEX:
			os << "V+";
			break;
		case constraint_type::EDGE:
			os << "E";
			break;
		case constraint_type::POSITIVE_EDGE:
			os <<"E+";
			break;
		case constraint_type::BARRIER:
			os << "B";
			break;
		case constraint_type::RANGE:
			os << "R";
		case constraint_type::GLENGTH:
			os << "G";
		case constraint_type::LEQLENGTH:
			os << "L";
	}
	os << ">";
	return os;
}


std::ostream& operator<<(std::ostream& os, const Conflict& conflict)
{
	switch (conflict.p)
	{
		case conflict_priority::CARDINAL:
			os << "cardinal ";
			break;
		case conflict_priority::SEMI:
			os << "semi-cardinal ";
			break;
		case conflict_priority::NON:
			os << "non-cardinal ";
			break;
        case conflict_priority::PRIORITY_COUNT:
            break;
    }
	switch (conflict.type)
	{
		case conflict_type::STANDARD:
			os << "standard";
			break;
		case conflict_type::RECTANGLE:
			os << "rectangle";
			break;
		case conflict_type::CORRIDOR:
			os << "corrdior";
			break;
		case conflict_type::TARGET:
			os << "target";
			break;
		case conflict_type::MUTEX:
			os << "mutex";
			break;
	    case conflict_type::TYPE_COUNT:
            break;
    }
	os << " conflict:  " << conflict.a1 << " with ";
	for (auto con : conflict.constraint1)
		os << con << ",";		
	os << " and " << conflict.a2 << " with ";
	for (auto con : conflict.constraint2)
		os << con << ",";		
	return os;
}

bool operator < (const Conflict& conflict1, const Conflict& conflict2) // return true if conflict2 has higher priority
{
	/*if (conflict1.type == conflict_type::TARGET && conflict2.type == conflict_type::TARGET)
	{
		if (conflict1.p > conflict2.p)
		    return true;
		else if (conflict1.p < conflict2.p)
		    return false;
        else
            return (conflict2.t < conflict1.t);
	}
	else if (conflict1.type == conflict_type::TARGET)
		return false;
	else if (conflict2.type == conflict_type::TARGET)
		return true;*/
	
	if (conflict1.p < conflict2.p)
		return false;
	else if (conflict1.p > conflict2.p)
		return true;
	else if (conflict1.type == conflict_type::MUTEX && conflict2.type != conflict_type::MUTEX)
		return false;
	else if (conflict1.type != conflict_type::MUTEX && conflict2.type == conflict_type::MUTEX)
		return true;
	else if (conflict1.type == conflict_type::TARGET && conflict2.type != conflict_type::TARGET)
	    return false;
    else if (conflict1.type != conflict_type::TARGET && conflict2.type == conflict_type::TARGET)
        return true;
	else if (conflict1.type == conflict_type::CORRIDOR && conflict2.type != conflict_type::CORRIDOR)
        return false;
    else if (conflict1.type != conflict_type::CORRIDOR && conflict2.type == conflict_type::CORRIDOR)
        return true;
	else if (conflict1.type == conflict_type::RECTANGLE && conflict2.type != conflict_type::RECTANGLE)
		return false;
	else if (conflict1.type != conflict_type::RECTANGLE && conflict2.type == conflict_type::RECTANGLE)
		return true;
	else if (conflict1.secondary_priority > conflict2.secondary_priority)
		return false;
	else if (conflict1.secondary_priority < conflict2.secondary_priority)
		return true;
	else return (rand() % 2);
}

