# PRO200 - Smidig prosjekt - Kolonial.no Svisj

The assignment was to build something technical for Kolonial.no - one of Norway's biggest grocery stores - to help increase their sales.

## Concept
The concept we came up with was an IoT device that brings the grocery store right into your kitchen. The device should connect to your wifi, and give you the chance to shop at your own convenience, by scanning barcodes for products you run out of. Took the last carton of milk? Scan it before you recycle the packaging and you'll have your inventory under control. This should also be a constant reminder that Kolonial.no is always ready to take your order, without the need for the mobile app or access to a computer/tablet.

## Svisj - the box

The box is 3D-printed, and houses the NodeMCU microcontroller (based on ESP8266), battery and LEDs to signal feedback from the server.

### Pictures of prototype

Attached are a few images of the finished prototype.
![Prototype - box](https://camo.githubusercontent.com/648860f665b79186a30580cffb675ade579db7f8/68747470733a2f2f692e696d6775722e636f6d2f45624c487744362e6a7067)
![Prototype - box inside](https://camo.githubusercontent.com/f28e7bc8900af5f7b23dd679d5b8635698bf33b2/68747470733a2f2f692e696d6775722e636f6d2f6e6d7252724a442e6a7067)


### Hardware diagram - Fritzing

Attached is hardware wire diagram, made with Fritzing.
![Alt Text](Koblingskjema/KoloBox_bb.png?raw=true, "Fritzing diagram")

### Code

The code running on the microcontroller is found in:
`/NodeMCU_kode/MASTER/MASTER.ino`

## The backend

As we did not gain access to the Kolonial.no API we were left to make a backend that emulates the functionality of the grocery store Kolonial.no runs today. We filled the database with a list of common household items, pictures and their barcode. It's also possible to add new items.

### Technical

The backend is made in the PHP framework Laravel and talks to a MySQL database. It also accepts incoming POST-requests from the Svisj box and sends feedback to the box depending on if the item exists in the database.

The code for the backend is found in:
`/web_api/`

The code also runs live at: [vrangum.com/kolonial](http://www.vrangum.com/kolonial)
