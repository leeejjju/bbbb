from flask import Flask

app = Flask(__name__) 
@app.route('/')

def index():
	return 'This is the main page'

@app.route('/iot')
def iot():
	return 'This is iotpage'

@app.route('/led/<state>')
def led(state):
	if state == 'on' or state == 'off':
		str= "Led is now '%s'." %state
	else:
		str= 'Invalid Pages'
	return str

if __name__ == '__main__': 
	print('Starting Webserver...')
	app.run(host='0.0.0.0')
