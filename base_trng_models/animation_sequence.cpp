#include "animation_sequence.h"

#include <iostream>
#include <string>
#include <iomanip>
#include <map>
#include <sstream>

#include "loader.h"

std::pair<AnimationCommand,std::string> ParseCommand(const std::string &command)
{
    if(command =="")
    {
        return std::make_pair(AnimationCommand::kNone,"");
    }
    std::map<std::string,AnimationCommand> commands;
    commands.insert(std::make_pair("message", AnimationCommand::kMessage));
    commands.insert(std::make_pair("strike", AnimationCommand::kStrike));
    commands.insert(std::make_pair("move", AnimationCommand::kMove));
    commands.insert(std::make_pair("rotate", AnimationCommand::kRotate));
    std::stringstream command_stream(command);
    AnimationCommand cmd_id = commands[LoaderUtility::GetFromStream<std::string>(command_stream)];
    std::string params;
    std::getline(command_stream, params);

    return std::make_pair(cmd_id,params);
}

std::string CommandToStream(std::pair<AnimationCommand,std::string> value)
{
    std::map<AnimationCommand,std::string> commands;
    commands.insert(std::make_pair(AnimationCommand::kNone,""));
    commands.insert(std::make_pair(AnimationCommand::kMessage,"message"));
    commands.insert(std::make_pair( AnimationCommand::kStrike,"strike"));
    commands.insert(std::make_pair( AnimationCommand::kMove,"move"));
    commands.insert(std::make_pair( AnimationCommand::kRotate,"rotate"));

    std::stringstream command_stream;
    command_stream << commands[value.first]<<" "<<value.second;
    return command_stream.str();
}

std::istream& operator>> ( std::istream& is, AnimationSequence & value)
{
	is>>value.start_frame>>value.end_frame>>value.m_loop>>value.m_jump;
    if(value.m_jump)
        is>>value.m_target_sequence;
    std::string tmp;
    is >>std::quoted(tmp);
    value.m_start_message = ParseCommand(tmp);
    is>>std::quoted(tmp);
    value.m_end_message = ParseCommand(tmp);
    //std::cout<<"animation "<<value;
}

std::ostream& operator << ( std::ostream& os, const AnimationSequence & value)
{
    os<<value.start_frame<<" "<<value.end_frame<<" "<<value.m_loop<<" "<<value.m_jump;
    if(value.m_jump) 
        os<<" "<<value.m_target_sequence;
        os<<" "<<std::quoted(CommandToStream(value.m_start_message))<<" "<<std::quoted(CommandToStream(value.m_end_message))<<"\n";
	return os;
}