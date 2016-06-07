var update_time = 100;//ms

function set_message_counts(table) {
    $("#"+table.table+"_table_count").text(table.nr_of_messages);
};

function get_message_count(table) {
    $.getJSON('nr_of_messages', {table: table}, function(server) {
        set_message_counts(server);
    });
};

function set_current_topic(topic = {"current_topic":""}){
    $("#current_topic").text(topic.current_topic);
};
function get_current_topic(table) {
    $.getJSON('current_topic', {}, function(server) {
        set_current_topic(server);
    }).fail(function(d) {
        set_current_topic();
    });
};

setInterval(function(){
    get_message_count("message");
    get_message_count("topic");
    get_message_count("payload");
    get_current_topic();
}, update_time);

window.onload = function () {
    var dps_messages = []; // dataPoints
    var dps_topics = [];
    var dps_payloads = [];
    var chart_message = new CanvasJS.Chart("chart_messages",{
        backgroundColor: "#424242",
        title :{
            text: "MQTT Total message count",
            fontColor: "#888888"
        },
        legend : {
            fontColor: "#888888",
        },
        axisX:{
            labelFontColor: "white",
        },
        axisY:{
            labelFontColor: "white",
        },
        data: [
            {
                type: "line",
                showInLegend: false,
                name: "messages",
                legendText: "# of messages",
                dataPoints: dps_messages,
                color: "red"
            }
        ]
    });
    var chart_topic = new CanvasJS.Chart("chart_topics",{
        backgroundColor: "#424242",
        title :{
            text: "MQTT Total topic count",
            fontColor: "#888888"
        },
        legend : {
            fontColor: "#888888",
        },
        axisX:{
            labelFontColor: "white",
        },
        axisY:{
            labelFontColor: "white",
        },
        data: [
            {
                type: "line",
                showInLegend: false,
                name: "topics",
                legendText: "# of topics",
                dataPoints: dps_topics,
                color: "#FFA500"
            }
        ]
    });
    var chart_payload = new CanvasJS.Chart("chart_payloads",{
        backgroundColor: "#424242",
        title :{
            text: "MQTT Total payload count",
            fontColor: "#888888"
        },
        legend : {
            fontColor: "#888888",
        },
        axisX:{
            labelFontColor: "white",
        },
        axisY:{
            labelFontColor: "white",
        },
        data: [
            {
                type: "line",
                showInLegend: false,
                name: "payloads",
                legendText: "# of payloads",
                dataPoints: dps_payloads,
                color: "#00FFFF"
            }
        ]
    });

    var xVal = 0;
    var yVal = 100;
    var updateInterval = update_time;
    var dataLength = 500; // number of dataPoints visible at any point

    var updateChart = function (count) {
        count = count || 1;
        // count is number of times loop runs to generate random dataPoints.
        for (var j = 0; j < count; j++) {
            dps_messages.push({
                x: xVal,
                y: parseInt($("#message_table_count").text())
            });
            dps_topics.push({
                x: xVal,
                y: parseInt($("#topic_table_count").text())
            });
            dps_payloads.push({
                x: xVal,
                y: parseInt($("#payload_table_count").text())
            });
            xVal++;
        };
        if (dps_messages.length > dataLength) {
            dps_messages.shift();
        }
        if (dps_topics.length > dataLength) {
            dps_topics.shift();
        }
        if (dps_payloads.length > dataLength) {
            dps_payloads.shift();
        }
        chart_message.render();
        chart_topic.render();
        chart_payload.render();
    };
    // generates first set of dataPoints
    updateChart(dataLength);
    // update chart after specified time.
	setInterval(function(){updateChart()}, updateInterval);
}
