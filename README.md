
---

# **BWKI24 - Plant Growth Optimizer**
Ein Projekt für den **Bundeswettbewerb Künstliche Intelligenz**.

In diesem Repository finden Sie den Code sowie die Dokumentation unseres Projekts zur Optimierung des Pflanzenwachstums mit Hilfe von KI. Unser Ziel ist es, das Pflanzenwachstum ressourcenschonend zu gestalten, indem wir ein neuronales Netz verwenden, das den Bewässerungsprozess in Echtzeit steuert.

---

## **Inhalt des Repositories**

1. [**Dokumentation des Anbaus**](#dokumentation-des-anbaus)  
   - Ablauf eines Wachstumszyklus
   - Ergebnisse der Experimente

2. [**Edge Detection Seed**](#edge-detection-seed)  
   - Samenkorndetektion zur Optimierung der relativen Samenposition

3. [**Neural Network**](#neural-network)  
   - Code unseres neuronalen Netzes zur Vorhersage der optimalen Wassermenge

4. [**Platine und Software**](#platine-und-software)  
   - Design und Software für die Data Collector Platine, die in Zukunft auch die direkte Steuerung der Bewässerung übernehmen wird

5. [**plantPage**](#plantpage)  
   - Webvisualisierung der Wachstumsdaten, um Zyklen grafisch darzustellen

---

## **Dokumentation des Anbaus**

Unser Projekt begann mit der standardisierten Aussaat von 10 g Kressesamen, um eine vergleichbare Basis für die Wachstumszyklen zu schaffen. Die Samen wurden entweder auf Watte oder Erde ausgesät. Dabei führten wir verschiedene Experimente mit Microgreens (Kresse und Radies) durch.

### **Ablauf eines Wachstumszyklus:**

1. **Abmessen der Kressesamen:**  
   <img src="/Dokumentation%20des%20Anbaus/Abmessen%20der%20Kressesamen.jpeg" alt="Abmessen der Kressesamen" height="250">

2. **Aussat der Samen auf Watte und Erde:**  
   <img src="/Dokumentation%20des%20Anbaus/Aussat%20der%20Kressesamen.jpeg" alt="Aussat der Kresse" height="250">
   <img src="/Dokumentation%20des%20Anbaus/Test%20aufbau%20in%20Gewächshaus.jpeg" alt="Aussat auf Erde" height="250">

3. **Tägliche Dokumentation des Wachstums (Radies, 5 Tage):**

   <div style="display: flex; justify-content: space-around;">
      <img src="/Dokumentation%20des%20Anbaus/Microgreens%20Test%20(Tag%201).jpeg" alt="Tag 1" height="250">
      <img src="/Dokumentation%20des%20Anbaus/Microgreens%20Test%20(Tag%202).jpeg" alt="Tag 2" height="250">
      <img src="/Dokumentation%20des%20Anbaus/Microgreens%20Test%20(Tag%203).jpeg" alt="Tag 3" height="250">
   </div>

   <div style="display: flex; justify-content: space-around;">
      <img src="/Dokumentation%20des%20Anbaus/Microgreens%20Test%20(Tag%204).jpeg" alt="Tag 4" height="250">
      <img src="/Dokumentation%20des%20Anbaus/Microgreens%20Test%20(Tag%205).jpeg" alt="Tag 5" height="250">
   </div>

4. **Ausgewachsene Kresse zum Vergleich:**  
   <img src="/Dokumentation%20des%20Anbaus/Kresse%20Ausgewachsen.jpeg" alt="Kresse Ausgewachsen" height="250">  
   <img src="/Dokumentation%20des%20Anbaus/Nahaufnahme%20Kresse.jpeg" alt="Kresse Nahaufnahme" height="250">

5. **Ertragsmessung:**  
   <div style="display: flex; justify-content: space-around;">
      <img src="/Dokumentation%20des%20Anbaus/Abwiegen%20des%20Ertrags%201.jpeg" alt="Ertrag 1" height="250">  
      <img src="/Dokumentation%20des%20Anbaus/Abwiegen%20des%20Ertrags%202.jpeg" alt="Ertrag 2" height="250">
   </div>

### **Ergebnisse der Experimente:**
- **Wassermenge und Ertrag:**  
  Durch die Optimierung der Wassermenge konnten wir eine Ertragssteigerung von bis zu 25 % erreichen. Dies wurde durch die präzise Steuerung der Wasserversorgung und die Analyse der Keimungsraten mit Bildverarbeitung ermöglicht.

- **Zukünftige Entwicklungen:**  
  Unsere neue **Data Collector Platine** soll in Zukunft nicht nur die Wassermenge in Echtzeit regulieren, sondern auch den pH-Wert und die Nährstoffdichte im Boden messen. Diese neue Version der Platine ist bereits entwickelt, aber zum Zeitpunkt der Abgabe noch nicht eingetroffen.

---

Hier ist der angepasste Code für die **Edge Detection Seed**-Sektion mit den hinzugefügten Bildern:

---

## **Edge Detection Seed**
In diesem Abschnitt unseres Repositories finden Sie den Code zur Samenkorndetektion. Mit dieser Technologie optimieren wir die relative Position der Samen, um eine bessere Keimung zu gewährleisten. Die Bildverarbeitungstechniken wie Sobel- und Canny-Algorithmen helfen dabei, die Konturen der Samen zu erkennen.

### **Bilder der Samenkorndetektion:**

<div style="display: flex; justify-content: space-around;">
   <!-- Original Seed Image -->
   <figure>
      <img src="/Edge%20Detection%20Seed/Seed.png" alt="Original Seed Image" height="250">
      <figcaption>Originales Bild des Samens</figcaption>
   </figure>

   <!-- Canny Edge Detection Output -->
   <figure>
      <img src="/Edge%20Detection%20Seed/Edge%20Detection%20Seed%20Output%20Canny.png" alt="Canny Edge Detection" height="250">
      <figcaption>Ergebnis der Canny-Detektion</figcaption>
   </figure>

   <!-- Sobel Edge Detection Output -->
   <figure>
      <img src="/Edge%20Detection%20Seed/Edge%20Detection%20Seed%20Output%20Sobel.png" alt="Sobel Edge Detection" height="250">
      <figcaption>Ergebnis der Sobel-Detektion</figcaption>
   </figure>
</div>

---

## **Neural Network**
Unser neuronales Netz besteht aus mehreren Schichten und wird genutzt, um basierend auf verschiedenen Umgebungsparametern (wie Temperatur, CO₂-Wert und Luftfeuchtigkeit) die optimale Wassermenge für den Pflanzenwachstumsprozess vorherzusagen.

```python
model = Sequential()
model.add(Dense(128, input_dim=5, activation='relu'))
model.add(Dense(64, activation='relu'))
model.add(Dense(1, activation='linear'))
model.compile(loss='mean_absolute_error', optimizer='adam', metrics=['mae'])
```
Das Modell wurde mit einem **Adam-Optimierer** trainiert, um den mittleren absoluten Fehler (MAE) zu minimieren und die Effizienz der Bewässerung zu maximieren.

---

## **Platine und Software**
Unsere speziell entwickelte **Data Collector Platine** sammelt und verarbeitet die Umgebungsdaten (wie Feuchtigkeit, CO₂, Temperatur) in Echtzeit und übermittelt sie an das neuronale Netz. Mit unserer **neuen Version** der Platine können zusätzliche Parameter wie pH-Wert und Nährstoffgehalt erfasst werden, was in Zukunft die Bewässerung noch präziser gestalten wird.

Die Software für die Platine ist ebenfalls in diesem Repository enthalten und ermöglicht die direkte Steuerung und Überwachung des Wachstumsprozesses.

---

## **plantPage**
Besuchen Sie unsere [alte Webvisualisierung](https://hou-mea.com/dataVisualizer/main.html), die einen Wachstumszyklus grafisch darstellt. Eine neue Version der Seite ist aktuell in der Entwicklung und kann lokal über `npm dev run` gestartet werden.

---

## **Wachstumsgif**
Hier ein animiertes GIF, das den Wachstumsprozess der Kresse über mehrere Tage zeigt:

<img src="/Wachstum.gif" alt="Wachstumsgif" height="400">

---

Dieses Repository enthält den vollständigen Code für unser Projekt, einschließlich der Wachstumsdaten, des neuronalen Netzes und der Platinen-Software. Weitere Informationen zu den einzelnen Komponenten finden Sie in den entsprechenden Unterordnern.

---
