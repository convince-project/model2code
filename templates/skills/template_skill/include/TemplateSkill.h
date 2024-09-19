# pragma once

#include <mutex>
#include <thread>
#include <rclcpp/rclcpp.hpp>
#include "$className$SM.h"
#include <bt_interfaces/msg/$skillTypeLC$_response.hpp>/*INTERFACES_LIST*/
/*INTERFACE*/
#include <$eventData.interfaceName$/srv/$eventData.functionNameSnakeCase$.hpp> /*END_INTERFACE*/
/*TICK*/
#include <bt_interfaces/srv/tick_$skillTypeLC$.hpp>/*END_TICK*/
/*HALT*/#include <bt_interfaces/srv/halt_$skillTypeLC$.hpp>/*END_HALT*/
/*DATAMODEL*/
#include "$skillName$SkillDataModel.h" /*END_DATAMODEL*/

#define SERVICE_TIMEOUT 8

enum class Status{
	undefined,/*ACTION*/
	running, /*END_ACTION*/
	success,
	failure
};

class $className$
{
public:
	$className$(std::string name );
	bool start(int argc, char * argv[]);
	static void spin(std::shared_ptr<rclcpp::Node> node);
	/*TICK_CMD*/
	void tick( [[maybe_unused]] const std::shared_ptr<bt_interfaces::srv::Tick$skillType$::Request> request,
			   std::shared_ptr<bt_interfaces::srv::Tick$skillType$::Response>      response);/*END_TICK_CMD*/
	/*HALT_CMD*/
	void halt( [[maybe_unused]] const std::shared_ptr<bt_interfaces::srv::HaltAction::Request> request,
			   [[maybe_unused]] std::shared_ptr<bt_interfaces::srv::HaltAction::Response> response);/*END_HALT_CMD*/
	/*TOPIC_CALLBACK_LIST_H*/
	/*TOPIC_CALLBACK_H*/void topic_callback_$eventData.functionName$(const $eventData.interfaceName$::SharedPtr msg);/*END_TOPIC_CALLBACK_H*/

private:
	std::shared_ptr<std::thread> m_threadSpin;
	std::shared_ptr<rclcpp::Node> m_node;
	std::mutex m_requestMutex;
	std::string m_name;
	$SMName$ m_stateMachine;
	/*TICK_RESPONSE*/std::atomic<Status> m_tickResult{Status::undefined};/*END_TICK_RESPONSE*/
	/*TICK_CMD*/rclcpp::Service<bt_interfaces::srv::Tick$skillType$>::SharedPtr m_tickService;/*END_TICK_CMD*/
	/*HALT_RESPONSE*/std::atomic<bool> m_haltResult{false};/*END_HALT_RESPONSE*/
	/*HALT_CMD*/rclcpp::Service<bt_interfaces::srv::HaltAction>::SharedPtr m_haltService;/*END_HALT_CMD*/
	/*DATAMODEL*/$skillName$SkillDataModel m_dataModel; /*END_DATAMODEL*/
	/*TOPIC_SUBSCRIPTIONS_LIST_H*/
	/*TOPIC_SUBSCRIPTION_H*/rclcpp::Subscription<$eventData.interfaceName$>::SharedPtr m_subscription_$eventData.functionName$;/*END_TOPIC_SUBSCRIPTION_H*/
};

