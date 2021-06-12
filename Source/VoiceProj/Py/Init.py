#Run seperately

import numpy as np
import gc
from flask import Flask, jsonify, request
import pickle
import tensorflow as tf
import tensorflow.keras as keras
import matplotlib.pyplot as plt
from tensorflow.python.keras.layers.core import Dropout
import cv2

app = Flask(__name__)
model_name = 'vmodel.h5'
model = None
gpus = tf.config.experimental.list_physical_devices('GPU')
tf.config.experimental.set_virtual_device_configuration(gpus[0], [tf.config.experimental.VirtualDeviceConfiguration(memory_limit=512)])


@app.route('/herr', methods=['GET'])
def hello():
    global model
    if model == None:
        initmodel()

    val = ""
    print(request.args)
    if 'id' in request.args:
        val += str(request.args['id']) + " "

    if model == None:
        return jsonify(svalue=val + ' model failed to load')

    #x = np.random.rand(1, 28, 28, 1)
    #y = model.predict(x)
    gc.collect()
    return jsonify(svalue=val + ' hello, here is your predicted input ' + str(np.argmax(y[0])))

ind = 0
@app.route('/image', methods=['POST'])
def classifymnist():
    global ind
    # print('\n\n\nrequest recieved')
    print(request.remote_addr)
    # print(request.data, "\n")
    
    img = np.frombuffer(request.data, np.uint8).reshape(32,32)
    img = cv2.resize(img, (28, 28), interpolation = cv2.INTER_AREA)
    img = np.where(img > 0, 255, 0).astype(np.uint8)
    #cv2.imwrite('img' + str(ind) + ".png",img.reshape(28, 28))
    img = img / 255
    global model
    if model == None:
        initmodel()

    predicted = model.predict(img.reshape(1, 28, 28, 1))
    #print(predicted, "\n", np.amax(predicted[0]), "\n", np.argmax(predicted[0]))
    gc.collect()
    return jsonify({
        "accuracy": str(np.amax(predicted)),
        "value": str(np.argmax(predicted))
    })
    


def main():
    app.run(debug=True, port=6502)

def initmodel():
    global model
    try:
        model = keras.models.load_model(model_name)
    except Exception as e:
        print(e)
    if model == None:
        (X_train, y_train), (X_test, y_test) = keras.datasets.mnist.load_data()
        for i in range(len(X_train)):
            X_train[i] = np.where(X_train[i] > 0, 1, 0)
        for i in range(len(X_test)):
            X_test[i] = np.where(X_test[i] > 0, 1, 0)
        print(y_train[0:15])
        y_train = np_utils.to_categorical(y_train)
        print(y_train[0:15])
        y_test = np_utils.to_categorical(y_test)
        X_train = X_train.reshape(-1, 28, 28, 1)
        X_test = X_test.reshape(-1, 28, 28, 1)
        model = keras.models.Sequential([
            keras.layers.Conv2D(32, 3, activation='relu', input_shape=(28,28,1)),
            keras.layers.MaxPool2D(2),
            keras.layers.Conv2D(64, 3, activation='relu'),
            keras.layers.MaxPool2D(2),
            keras.layers.Flatten(),
            keras.layers.Dense(100, activation='relu'),
            keras.layers.Dense(10, activation='sigmoid'),
        ])
        model.compile(loss = 'categorical_crossentropy', optimizer = 'adam', metrics = ['accuracy'])
        model.fit(X_train, y_train, epochs=10, validation_data=(X_test, y_test))
    

if __name__ == "__main__":
    initmodel()
    # import tensorflow as tf
    # import tensorflow.keras as keras
    


    pass
    #run model
    #trian
    #blabla


main()
