# lab7_rpi_flask_get_temp_cds.py
from flask import Flask, render_template
from flask_mqtt import Mqtt
import time

app = Flask(__name__)
app.config['MQTT_BROKER_URL'] = 'sweetdream.iptime.org'
app.config['MQTT_BROKER_PORT'] = 1883
app.config['MQTT_USERNAME'] = 'iot'
app.config['MQTT_PASSWORD'] = 'csee1414'
mqtt = Mqtt(app)

pub_topic = 'iot/22100579'
sub_topic_dht22 = 'iot/22100579/dht22'
sub_topic_cds = 'iot/22100579/cds'
mqtt_message = ''

print('@@ Use URL: /(led, dht22, cds)')

@app.route('/')
def home():
	return render_template('index_test.html')

@app.route('/iot/<cmd>')
def get_command(cmd):
	global mqtt_message
	if cmd == 'led':
		mqtt.publish(pub_topic, cmd)
		time.sleep(1)
		return render_template('index_test.html')
	elif cmd == 'usbledon':
		mqtt.publish(pub_topic, cmd)
		time.sleep(1)
		return render_template('index_test.html')
	elif cmd == 'usbledoff':
		mqtt.publish(pub_topic, cmd)
		time.sleep(1)
		return render_template('index_test.html')
	elif cmd == 'dht22':
		mqtt.publish(sub_topic_dht22, cmd)
		time.sleep(1)
		return render_template('index_test.html')
	elif cmd == 'cds':
		mqtt.publish(sub_topic_cds, cmd)
		time.sleep(1)
		return render_template('index_test.html')

@mqtt.on_connect()
def handle_connect(client, userdata, flags, rc):
	mqtt.subscribe(sub_topic_dht22)
	mqtt.subscribe(sub_topic_cds)

@mqtt.on_message()
def handle_mqtt_message(client, userdata, message):
	global mqtt_message
	#topic = message.topic.decode('utf-8')
	topic = message.topic
	payload = message.payload.decode('utf-8')
	mqtt_message = payload
	print("Topic: " + topic + " message: " + mqtt_message)
	
#@app.route('/led')
#def led_toggle():
#	mqtt.publish('iot/22100579', 'led')
#	return render_template('index_lab7.html')

@app.route('/led/on')
def led_on():
	mqtt.publish(pub_topic, 'ledon')
	return render_template('index_lab7.html')

@app.route('/led/off')
def led_off():
	mqtt.publish(pub_topic, 'ledoff')
	return render_template('index_lab7.html')

if __name__ == '__main__':
	app.run(host = '0.0.0.0', port=80, debug=False)



