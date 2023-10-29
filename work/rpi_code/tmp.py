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

pub_topic = 'iot/1995/22100579'
sub_topic_dht22 = 'iot/1995/dht22'
sub_topic_cds = 'iot/1995/cds'
mqtt_message = ''

print('@@ Use URL: /iot/1995/(led, dht22, cds)')

@app.route('/')
def home():
	return render_template('index.html')
@app.route('iot/1995/<cmd>')
def get_command(cmd):
	global mqtt_message
	if cmd == 'led':
		mqtt.publish(pub_topic, cmd)
time.sleep(1)
	elif cmd == 'dht22':
		mqtt.publish(sub_topic_dht22, cmd)
		time.sleep(1)
		return render_template('index_lab7.html', result='LED toggled')
	elif cmd == 'cds':
		mqtt.publish(sub_topic_cds, cmd)
		time.sleep(1)
		return render_template('index_lab7.html', result='LED toggled')

@mqtt.on_connect()
def handle_connect(client, userdata, flags, rc):

@mqtt_on_message()
def handle_mqtt_message(client, userdata, message):
	global mqtt message
	topic = message.topic.decode('utf-8')
	
		

@app.route('/led')
def led_toggle():
	mqtt.publish('iot/22100579', 'led')
	return render_template('index.html')

@app.route('/led/on')
def led_on():
	mqtt.publish('iot/22100579', 'ledon')
	return render_template('index.html')

@app.route('/led/off')
def led_off():
	mqtt.publish('iot/22100579', 'ledoff')
	return render_template('index.html')

@app.route('/iot/nth405/cds')
def get_light_intensity():
	mqtt.publish('iot/22100579', 'cds')
	return render_template('index.html')

@app.route('/iot/nth405/dht22')
def get_temp_hum():
	mqtt.publish('iot/22100579', 'dht22')
	return render_template('index.html')


if __name__ == '__main__':
	app.run(host = '0.0.0.0', debug=False)



