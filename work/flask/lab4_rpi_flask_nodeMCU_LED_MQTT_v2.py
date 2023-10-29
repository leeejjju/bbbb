# lab4_rpi_flask_nodeMCU_LED_MQTT_v2.py

from flask import Flask, render_template
from flask_mqtt import Mqtt

app = Flask(__name__)
#app.config['MQTT_BROKER_URL'] = '203.252.106.154'
app.config['MQTT_BROKER_URL'] = '192.168.137.140'
app.config['MQTT_BROKER_PORT'] = 1883

#app.config['MQTT_USERNAME'] = 'iot'
#app.config['MQTT_PASSWORD'] = 'csee1414'

mqtt = Mqtt(app)

@app.route('/led')
def main():
	return render_template('index.html')

@app.route('/led/toggle')
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



