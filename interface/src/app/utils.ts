import { Piece } from './editor/piece';
import * as myGlobals from './editor/globals';

         ///////////////////////////////
        // Déclaration des fonctions //
       ///////////////////////////////

export function goSend(m : myGlobals.mqttID): void {
    var etage1 = document.getElementById("color_etage1");
    var etage2 = document.getElementById("color_etage2");
    var etage3 = document.getElementById("color_etage3");

    var message_etage1 = sendEtage(etage1,"etage1_down","etage1_middle","etage1_top");
    var message_etage2 = sendEtage(etage2,"etage2_down","etage2_middle","etage2_top");
    var message_etage3 = sendEtage(etage3,"etage3_down","etage3_middle","etage3_top");
    console.log("Message %s :",message_etage1);
    console.log("Message %s :",message_etage2);
    console.log("Message %s :",message_etage3);
    m.client.publish(m.macID+"/ACK","1");
    m.client.publish(m.macID+"/Etage1", message_etage1);
    m.client.publish(m.macID+"/Etage2", message_etage2);
    m.client.publish(m.macID+"/Etage3", message_etage3);
    m.client.publish(m.macID+"/ACK","0");
}

export function Disconnect(m : myGlobals.mqttID): void {
    m.client.end();
}


export function sendEtage(etage,down,middle,top){

    // Initialisation de l'éclairage //
    etage.style.backgroundColor = "silver";

    // TABLEAU DE COULEURS TOP //
    var tab_color_top = new Array();
    tab_color_top = tableauCouleur(top);
    var tab_color_top_len = tab_color_top.length;

    // TABLEAU DE COULEURS DOWN //
    var tab_color_down = new Array();
    tab_color_down = tableauCouleur(down);
    var tab_color_down_len = tab_color_down.length;

    // TABLEAU DES IDs DES FONCTIONS //
    var tab_id = new Array();
    tab_id = tableauId(middle);
    var tab_id_len = tab_id.length;

    // TABLEAU DES PIECES FONCTIONS //
    var tab = new Array();
    tab = tableauPiece(tab_id,tab_id_len,tab_color_top,tab_color_top_len,tab_color_down,tab_color_down_len);
    if(tab==null){
        alert('Il y a une erreur dans ta programmation');
    }
    else{
        // CREATION DE LA CHAINE //
        var tempo = getTempo()*1000;    // Récupération de la durée des pièces
        var tab_len = tab.length;
        var chaine = creerChaineProtocole(tab,tab_len,tempo);
        return chaine;
    }
}



// Fonction principale faisant le traitement pour un étage passé en paramètre

export function playEtage(etage,down,middle,top){

    // Initialisation de l'éclairage //
    etage.style.backgroundColor = "silver";

    // TABLEAU DE COULEURS TOP //
    var tab_color_top = new Array();
    tab_color_top = tableauCouleur(top);
    var tab_color_top_len = tab_color_top.length;

    // TABLEAU DE COULEURS DOWN //
    var tab_color_down = new Array();
    tab_color_down = tableauCouleur(down);
    var tab_color_down_len = tab_color_down.length;

    // TABLEAU DES IDs DES FONCTIONS //
    var tab_id = new Array();
    tab_id = tableauId(middle);
    var tab_id_len = tab_id.length;

    // TABLEAU DES PIECES FONCTIONS //
    var tab = new Array();
    tab = tableauPiece(tab_id,tab_id_len,tab_color_top,tab_color_top_len,tab_color_down,tab_color_down_len);
    if(tab==null){
        alert('Il y a une erreur dans ta programmation');
    }
    else{
        // ALLUMAGE DE LA TOUR //
        var tempo = getTempo()*1000;    // Récupération de la durée des pièces
        var tab_len = tab.length;
        var i = 0;
        playTour(etage,tab,tab_len,i,tempo);
    }
}



// Fonction retournant un tableau de couleurs

export function tableauCouleur(id_etage : string) {
    var elmt = document.getElementById(id_etage).getElementsByClassName("couleur_piece"), elmt_Len = elmt.length;
    var tab_color = new Array(elmt_Len);   // Déclaration du tableau de couleurs

    for (var i = 0; i < elmt_Len; i++) {   // Récupération des pièces de couleurs
        var style = elmt[i].getAttribute("style").substring(5,12);
        tab_color[i]=hexToRGB(style); // Mise à jour du tableau avec les styles de couleurs (Par exemple "rgb(25, 255, 255)"
    }

    return tab_color;   // Retour du tableau
}

export function hexToRGB(hex){
  var r = hexToR(hex);
  var g = hexToG(hex);
  var b = hexToB(hex);
  var rgb="rgb("+r+", "+g+", "+b+")";
  return rgb;
}

function hexToR(h) {return parseInt((cutHex(h)).substring(0,2),16)}
function hexToG(h) {return parseInt((cutHex(h)).substring(2,4),16)}
function hexToB(h) {return parseInt((cutHex(h)).substring(4,6),16)}
function cutHex(h) {return (h.charAt(0)=="#") ? h.substring(1,7):h}



// Fonction retournant un tableau des  IDs des pièces fonctions

export function tableauId(id_etage : string) {
    var elmt = document.getElementById(id_etage).getElementsByClassName("fonct"), elmt_Len = elmt.length;
    var tab_id = new Array(elmt_Len);   // Déclaration du tableau d'IDs

    for (var i = 0; i < elmt_Len; i++) {   // Récupération des pièces fonctions
        tab_id[i] = elmt[i].id; // Récupération de l'ID des pièces
    }

    return tab_id;   // Retour du tableau
}



// Fonction retournant le tableau des pièces fonctions avec Style et Id
export function tableauPiece(tab_id, tab_id_len, tab_color_top, tab_color_top_len,tab_color_down, tab_color_down_len)
{
    // Vérification du bon nombre de pièces couleurs
    var nb_piece = 0;
    var nb_piece_double = 0;
    for (var i = 0; i < tab_id_len; i++) {
        if(tab_id[i]!=="Eteindre"){
            nb_piece++;
        }

        if(tab_id[i]==="Degrader"){
            nb_piece_double++;
        }
    }
    if((nb_piece!==tab_color_top_len)||(nb_piece_double!==tab_color_down_len)){
        console.log("ERROR");
        return null;
    }

    else
    {
        var tab_eta = new Array(tab_id_len);
        var k=0;
        var l=0;
        for (var i = 0; i < tab_id_len; i++) {
            if(tab_id[i]==="Allumer"){
                tab_eta[i]=new Piece(tab_color_top[k],"null","Allumer");
                k++;
            }

            else if(tab_id[i]==="Eteindre"){
                tab_eta[i]=new Piece("silver","null","Eteindre");
            }

            else if(tab_id[i]==="Degrader"){
                tab_eta[i]=new Piece(tab_color_top[k],tab_color_down[l],"Degrader");
                k++;
                l++;
            }

            else if(tab_id[i]==="Clignoter"){
                tab_eta[i]=new Piece(tab_color_top[k],"null","Clignoter");
                k++;
            }
        }

        return tab_eta;
    }
}



// Fonction allumant les étages de la tour passés en paramètres
export function playTour(etage,tab_etage,tab_etage_len,i,tempo) {
    if((i==tab_etage_len)||myGlobals.stop) {
        return 0;
    }

    else {
        if(tab_etage[i].getId()==="Allumer"){
            allumerTour(etage,tab_etage[i]);
        }

        else if(tab_etage[i].getId()==="Eteindre"){
            allumerTour(etage,tab_etage[i]);
        }

        else if(tab_etage[i].getId()==="Degrader"){
            degraderTour(etage,tab_etage[i],120,6*tempo/1000);
        }

        else if(tab_etage[i].getId()==="Clignoter"){
            var nb_clignotement = 5;
            var tempo_cligno = tempo/nb_clignotement;
            clignoterTour(etage,tab_etage[i],nb_clignotement,tempo_cligno);
        }

        i++;
        setTimeout(playTour, tempo,etage,tab_etage,tab_etage_len,i,tempo); // On rappelle la fonction allumerTour avec la temporisation en milisecondes
    }
}



// Fonction appelé lorsque nous avons une piece ALLUMER et ETEINDRE
export function allumerTour(etage,piece){
    console.log("%s",piece.getId());
    etage.style.backgroundColor = piece.getStyle(); // Allumage de l'étage avec le style de la fonction
}



// Fonction appelé lorsque nous avons une piece DEGRADER
export function degraderTour(etage,piece,nbTrans,delay){
    console.log("%s",piece.getId());
    var styleTop = piece.getStyle().slice(4,-1).split(','); // Récupérer les couleurs rouge, vert, bleu dans 3 variables différentes
    var styleDown = piece.getStyleDegra().slice(4,-1).split(',');

    var tab_top = [parseInt(styleTop[0],10),parseInt(styleTop[1],10),parseInt(styleTop[2],10)];
    var tab_down = [parseInt(styleDown[0],10),parseInt(styleDown[1],10),parseInt(styleDown[2],10)];

    var delay = delay || 100; // en millisecondes c ets le temps qui passe entre chaque execution. a diminuer si la fluidité n est pas au rendez vous
    var nbTrans = nbTrans || 20; // le nombre de transitions...a augmenter si ce n ets pas assez fluide
    var startColor = tab_top || [255,255,255]; // remplacer par les chiffres qui vont bien
    var stopColor = tab_down || [255,255,255]; // pareil

    // maintenant il faut calculer le delta de chaque couleur
    var rDelta = Math.floor((stopColor[0]-startColor[0])/nbTrans);
    var gDelta = Math.floor((stopColor[1]-startColor[1])/nbTrans);
    var bDelta = Math.floor((stopColor[2]-startColor[2])/nbTrans);

     // on cree les variables ou vont etre stockees les couleurs temporaires
    var rTemp = startColor[0];
    var gTemp = startColor[1];
    var bTemp = startColor[2];

    // un petit setInterval pour que la fonction se repete d elle meme
    var interval = setInterval(function() {
        rTemp += rDelta;
        gTemp += gDelta;
        bTemp += bDelta;
        etage.style.backgroundColor = "rgb("+rTemp+","+gTemp+","+bTemp+" )";
        if (rTemp<0 || gTemp<0 || bTemp<0|| rTemp>255 || gTemp>255 || bTemp>255 ) {
            clearInterval(interval);
            etage.style.backgroundColor = "rgb("+stopColor[0]+","+stopColor[1]+","+stopColor[2]+" )";
        }
    },delay);
}



// Fonction appelé lorsque nous avons une piece CLIGNOTER
export function clignoterTour(etage,piece,nb_cligno,tempo){
    console.log("%s",piece.getId());
    if(nb_cligno === 0){
        return 0;
    }

    else{
        var style = getComputedStyle(etage,null).backgroundColor; // Récupération de la couleur des pièces_couleurs
        if(style == piece.getStyle()){
             etage.style.backgroundColor = "silver";
        }

        else{
            etage.style.backgroundColor = piece.getStyle();
        }

        nb_cligno--;
        setTimeout(clignoterTour, tempo,etage,piece,nb_cligno,tempo);
    }
}



// Fonction qui nous renvoie la durée que dois avoir les pièces, en secondes
export function getTempo(){
    return (parseFloat((<HTMLInputElement>document.getElementById("duree_input")).value));
}



// Fonction qui créé la chaine de caractère que nous allons envoyer sur le serveur MQTT. Un protocole a été défini pour coder cette chaine et la décoder facilement
export function creerChaineProtocole(tab_etage,tab_etage_len,tempo){
    //Protocole :
    //  T=Temporisation
    //  L=Allumer
    //  O=Eteindre
    //  R=Degrader, S=Degrader deuxième couleur
    //  P=Clignoter
    //  W=Fin

    var chaine;
    if(tab_etage_len===0)
    {
        chaine = "null";
    }

    else {
        chaine = "T"+tempo;
        for (var i = 0; i < tab_etage_len; i++){
            if(tab_etage[i].getId()==="Allumer"){
                chaine = chaine+"L"+rgbToHex(tab_etage[i].getStyle());
            }

            else if(tab_etage[i].getId()==="Eteindre"){
                chaine = chaine+"O"+"000000";
            }

            else if(tab_etage[i].getId()==="Degrader"){
                chaine = chaine+"R"+rgbToHex(tab_etage[i].getStyle())+"S"+rgbToHex(tab_etage[i].getStyleDegra());
            }

            else if(tab_etage[i].getId()==="Clignoter"){
                chaine = chaine+"P"+rgbToHex(tab_etage[i].getStyle());
            }
        }
    }
    chaine = chaine+"W" // Fin de la chaine
    return chaine;
}



// Fonction qui convertie une couleur RGB en HEXA. Utilisée dans la fonction creerChaineProtocole
export function rgbToHex(style) {
    var stylergb = style.slice(4,-1).split(',');

    var tab_rgb = [parseInt(stylergb[0],10),parseInt(stylergb[1],10),parseInt(stylergb[2],10)];
    return ((1 << 24) + (tab_rgb[0] << 16) + (tab_rgb[1] << 8) + tab_rgb[2]).toString(16).slice(1);
}



// Fonction d'annimation de la barre de temps
export function barreMove(elmt) {
    var tempo = getTempo()*1000;    // Récupération de la durée des pièces
    var pos = 41;
    var id = setInterval(frame, tempo/61);
    function frame() {
        if ((pos == 955)||(myGlobals.stop)) {
            clearInterval(id);
        }

        else {
            pos++;
            elmt.style.left = pos + 'px';
        }
    }
}



export function open_infos() {
    var width = 500;
    var height = 300;
    if(window.innerWidth){
        var left = (window.innerWidth-width)/2;
        var top = (window.innerHeight-height)/2;
    }
    else
    {
        var left = (document.body.clientWidth-width)/2;
        var top = (document.body.clientHeight-height)/2;
    }
    window.open('/connect','nom_de_ma_popup','menubar=no, scrollbars=no, top='+top+', left='+left+', width='+width+', height='+height+'');
}
