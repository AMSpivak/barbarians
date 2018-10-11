#include "animation_sequence.h"

#include <iostream>
#include <string>
#include <iomanip>
#include <map>
#include <sstream>

#include "loader.h"

std::map<std::string,AnimationCommand> commands = {
                                                        {"message", AnimationCommand::kMessage},
                                                        {"strike", AnimationCommand::kStrike},
                                                        {"move", AnimationCommand::kMove},
                                                        {"rotate", AnimationCommand::kRotate},
                                                        {"use", AnimationCommand::kUse},
                                                        {"executed", AnimationCommand::kExecuted}

                                                    };

std::map<AnimationCommand,std::string> command_names = {
                                                        { AnimationCommand::kMessage,"message"},
                                                        { AnimationCommand::kStrike,"strike"},
                                                        { AnimationCommand::kMove,"move"},
                                                        { AnimationCommand::kRotate,"rotate",},
                                                        {AnimationCommand::kUse,"use" },
                                                        { AnimationCommand::kExecuted,"executed"}

                                                    };

std::pair<AnimationCommand,std::string> ParseCommand(const std::string &command)
{
    if(command =="")
    {
        return std::make_pair(AnimationCommand::kNone,"");
    }
    std::stringstream command_stream(command);
    AnimationCommand cmd_id = commands[LoaderUtility::GetFromStream<std::string>(command_stream)];
    std::string params;
    std::string dummy;
    
    std::getline(std::getline(command_stream,dummy,' '), params);

    return std::make_pair(cmd_id,params);
}



std::string CommandToStream(std::pair<AnimationCommand,std::string> value)
{
    std::stringstream command_stream;
    command_stream << command_names[value.first]<<" "<<value.second;
    return command_stream.str();
}

std::istream& operator>> ( std::istream& is, AnimationSequence & value)
{
    std::string skip; 
	is>>value.start_frame>>value.end_frame>>value.m_loop>>value.m_jump;
    if(value.m_jump)
        is>>value.m_target_sequence;
    std::string tmp;
    //is >>std::quoted(tmp);
    std::getline(std::getline(is, skip, '"'), tmp, '"') ;

    value.m_start_message = ParseCommand(tmp);
    std::cout << "<m_start_message------->"<<tmp<<"!\n";
    //is>>std::quoted(tmp);
    std::getline(std::getline(is, skip, '"'), tmp, '"') ;

    value.m_frame_message = ParseCommand(tmp);
    std::cout << "<m_frame_message------->"<<tmp<<"!\n";
    //is>>std::quoted(tmp);
    std::getline(std::getline(is, skip, '"'), tmp, '"') ;
    value.m_end_message = ParseCommand(tmp);
    std::cout << "<m_end_message------->"<<tmp<<"!\n";
    std::cout << "<commands-------> \n";
    tmp = "";
    while(!is.eof())
    {
        //is >>std::quoted(tmp);
        std::getline(std::getline(is, skip, '"'), tmp, '"') ;
        if(tmp!="")
        {
            value.jumps.insert( ParseCommand(tmp));
            std::cout << "<command><" << tmp <<"> \n";
        }
        tmp="";
    }
    std::cout << "<commands-------> \n";

    //std::cout<<"animation "<<value;
}

std::ostream& operator << ( std::ostream& os, const AnimationSequence & value)
{
    os<<value.start_frame<<" "<<value.end_frame<<" "<<value.m_loop<<" "<<value.m_jump;
    if(value.m_jump) 
        os<<" "<<value.m_target_sequence;
        os<<" "<<std::quoted(CommandToStream(value.m_start_message))<<" "
        <<std::quoted(CommandToStream(value.m_frame_message))<<" "
        <<std::quoted(CommandToStream(value.m_end_message));

        os<<"\n";
	return os;
}