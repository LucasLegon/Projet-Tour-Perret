// Orientation de la tour //

export var orientation_letter = ['N','E','S','W'];
export var indice_letter;
export function setIndiceLetter(newValue: number) {
    indice_letter = newValue;
}


// Arrêter les annimations //

export var stop;
export function setStop(newValue: Boolean) {
    stop = newValue;
}


// MQTT_ID //

export class mqttID {
  client;
  macID: number;
  isConnected : number;

}