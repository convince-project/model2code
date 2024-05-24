#include "$className$.h"
#include <future>
#include <QTimer>
#include <QDebug>
#include <QTime>
#include <iostream>
#include <QStateMachine>

#include <type_traits>

template<typename T>
T convert(const std::string& str) {
    if constexpr (std::is_same_v<T, int>) {
        return std::stoi(str);
    } else if constexpr (std::is_same_v<T, double>) {
        return std::stod(str);
    } else if constexpr (std::is_same_v<T, float>) {
        return std::stof(str);
    } 
    else if constexpr (std::is_same_v<T, bool>) { 
        if (str == "true" || str == "1") { 
            return true; 
        } else if (str == "false" || str == "0") { 
            return false; 
        } else { 
            throw std::invalid_argument("Invalid boolean value"); 
        } 
    } 
    else if constexpr (std::is_same_v<T, std::string>) {
        return str;
    }
    else {
        throw std::invalid_argument("Unsupported type conversion");
    }
}

$className$::$className$(std::string name ) :
		m_name(std::move(name))
{
    /*DATAMODEL*/m_stateMachine.setDataModel(&m_dataModel);/*END_DATAMODEL*/
}

void $className$::spin(std::shared_ptr<rclcpp::Node> node)
{
	rclcpp::spin(node);
	rclcpp::shutdown();
}

bool $className$::start(int argc, char*argv[])
{
	if(!rclcpp::ok())
	{
		rclcpp::init(/*argc*/ argc, /*argv*/ argv);
	}

	m_node = rclcpp::Node::make_shared(m_name + "Skill");
	RCLCPP_DEBUG_STREAM(m_node->get_logger(), "$className$::start");
	std::cout << "$className$::start";

    /*TICK*/
	m_tickService = m_node->create_service<bt_interfaces::srv::Tick$skillType$>(m_name + "Skill/tick",
                                                                           	std::bind(&$className$::tick,
                                                                           	this,
                                                                           	std::placeholders::_1,
                                                                           	std::placeholders::_2));/*END_TICK*/
    /*HALT*/
	m_haltService = m_node->create_service<bt_interfaces::srv::Halt$skillType$>(m_name + "Skill/halt",
                                                                            	std::bind(&$className$::halt,
                                                                            	this,
                                                                            	std::placeholders::_1,
                                                                            	std::placeholders::_2));/*END_HALT*/
    /*SEND_EVENT_LIST*//*SEND_EVENT_SRV*/
    m_stateMachine.connectToEvent("$eventData.event$", [this]([[maybe_unused]]const QScxmlEvent & event){
        std::shared_ptr<rclcpp::Node> $eventData.nodeName$ = rclcpp::Node::make_shared(m_name + "SkillNode$eventData.functionName$");
        std::shared_ptr<rclcpp::Client<$eventData.interfaceName$::srv::$eventData.functionName$>> $eventData.clientName$ = $eventData.nodeName$->create_client<$eventData.interfaceName$::srv::$eventData.functionName$>($eventData.serverName$);
        auto request = std::make_shared<$eventData.interfaceName$::srv::$eventData.functionName$::Request>();
        auto eventParams = event.data().toMap();
        /*PARAM_LIST*//*PARAM*/
        request->$IT->FIRST$ = convert<decltype(request->$IT->FIRST$)>(eventParams["$IT->FIRST$"].toString().toStdString());/*END_PARAM*/
        bool wait_succeded{true};
        int retries = 0;
        while (!$eventData.clientName$->wait_for_service(std::chrono::seconds(1))) {
            if (!rclcpp::ok()) {
                RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the service '$eventData.functionName$'. Exiting.");
                wait_succeded = false;
                break;
            } 
            retries++;
            if(retries == SERVICE_TIMEOUT) {
               RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Timed out while waiting for the service '$eventData.functionName$'.");
               wait_succeded = false;
               break;
            }
        }
        if (wait_succeded) {                                                                   
            auto result = $eventData.clientName$->async_send_request(request);
            const std::chrono::seconds timeout_duration(SERVICE_TIMEOUT);
            auto futureResult = rclcpp::spin_until_future_complete($eventData.nodeName$, result, timeout_duration);
            if (futureResult == rclcpp::FutureReturnCode::SUCCESS) 
            {
               auto response = result.get();
               if( response->is_ok ==true) {
                   QVariantMap data;
                   data.insert("result", "SUCCESS");/*RETURN_PARAM_LIST*//*RETURN_PARAM*/
                   data.insert("$eventData.interfaceDataField$", response->$eventData.interfaceDataField$/*STATUS*/.status/*END_STATUS*/);/*END_RETURN_PARAM*/
                   m_stateMachine.submitEvent("$eventData.componentName$.$eventData.functionName$.Return", data);
                   RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "$eventData.componentName$.$eventData.functionName$.Return");
                   return;
               }
           }
           else if(futureResult == rclcpp::FutureReturnCode::TIMEOUT){
               RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Timed out while future complete for the service '$eventData.functionName$'.");
           }
        }
       QVariantMap data;
       data.insert("result", "FAILURE");
       m_stateMachine.submitEvent("$eventData.componentName$.$eventData.functionName$.Return", data);
       RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "$eventData.componentName$.$eventData.functionName$.Return");
    });/*END_SEND_EVENT_SRV*/
    /*TICK_RESPONSE*/
	m_stateMachine.connectToEvent("TICK_RESPONSE", [this]([[maybe_unused]]const QScxmlEvent & event){
		RCLCPP_INFO(m_node->get_logger(), "$className$::tickReturn %s", event.data().toMap()["result"].toString().toStdString().c_str());
		std::string result = event.data().toMap()["result"].toString().toStdString();
		if (result == "SUCCESS" )
		{
			m_tickResult.store(Status::success);
		}/*ACTION*/
		else if (result == "RUNNING" )
		{
			m_tickResult.store(Status::running);
		}/*END_ACTION*/
		else if (result == "FAILURE" )
		{ 
			m_tickResult.store(Status::failure);
		}
	});/*END_TICK_RESPONSE*/
    /*HALT_RESPONSE*/
	m_stateMachine.connectToEvent("HALT_RESPONSE", [this]([[maybe_unused]]const QScxmlEvent & event){
		RCLCPP_INFO(m_node->get_logger(), "$className$::haltresponse");
		m_haltResult.store(true);
	});/*END_HALT_RESPONSE*/

	m_stateMachine.start();
	m_threadSpin = std::make_shared<std::thread>(spin, m_node);

	return true;
}
/*TICK_CMD*/
void $className$::tick( [[maybe_unused]] const std::shared_ptr<bt_interfaces::srv::Tick$skillType$::Request> request,
                                std::shared_ptr<bt_interfaces::srv::Tick$skillType$::Response>      response)
{
    std::lock_guard<std::mutex> lock(m_requestMutex);
    RCLCPP_INFO(m_node->get_logger(), "$className$::tick");
    auto message = bt_interfaces::msg::$skillType$Response();
    m_tickResult.store(Status::undefined);
    m_stateMachine.submitEvent("CMD_TICK");
   
    while(m_tickResult.load()== Status::undefined) {
        std::this_thread::sleep_for (std::chrono::milliseconds(100));
    }
    switch(m_tickResult.load()) 
    {
        /*ACTION*/case Status::running:
            response->status.status = message.SKILL_RUNNING;
            break;/*END_ACTION*/
        case Status::failure:
            response->status.status = message.SKILL_FAILURE;
            break;
        case Status::success:
            response->status.status = message.SKILL_SUCCESS;
            break;            
    }
    RCLCPP_INFO(m_node->get_logger(), "$className$::tickDone");
    response->is_ok = true;
}/*END_TICK_CMD*/
/*HALT_CMD*/
void $className$::halt( [[maybe_unused]] const std::shared_ptr<bt_interfaces::srv::Halt$skillType$::Request> request,
    [[maybe_unused]] std::shared_ptr<bt_interfaces::srv::Halt$skillType$::Response> response)
{
    std::lock_guard<std::mutex> lock(m_requestMutex);
    RCLCPP_INFO(m_node->get_logger(), "$className$::halt");
    m_haltResult.store(false);
    m_stateMachine.submitEvent("CMD_HALT");
    while(!m_haltResult.load()) {
        std::this_thread::sleep_for (std::chrono::milliseconds(100));
    }
    RCLCPP_INFO(m_node->get_logger(), "$className$::haltDone");
    response->is_ok = true;
}
/*END_HALT_CMD*/