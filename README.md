# Water Sommelier Session Manager

## Over het project

C++ desktop applicatie gebouwd met **Raylib** en een **custom UI library**.  
Doel: beheren van waterproef-sessies voor 5–8 deelnemers met **barcode-gestuurde core**.  
Opmerkingen per glas worden via toetsenbord ingevoerd.

---

## Functionaliteit

- **Deelnemers:** naam + barcode  
- **Glazenbeheer:** per deelnemer  
- **Zoutregistratie:** barcode + aantal druppels  
- **Scoring:** per glas  
- **Data opslag:** CSV (opslaan en inlezen van deelnemers, glazen, zouten, scores)  
- **Visuele stimulatie:** boid-achtige simulatie van zouten in glazen  

---

## Sessieverloop

1. Barcode scannen en naam invoeren  
2. Voor elk glas:  
   - Scan je barcode  
   - Scan zout  
   - Voeg druppels toe  
   - Geef score/opmerking via toetsenbord  
3. Alles wordt automatisch opgeslagen in CSV  

---

## Besturing

- Core interactie: **barcode scanner**  
- Opmerkingen en scores: **toetsenbord**  
- Navigatie: **Tab / Reverse Tab**  

---

## Web & QR-code

- Na sessie start lokale server  
- QR-code genereert formulier voor extra data  
- Deelnemers kunnen via gsm of hoofdcomputer invoeren  

---

## Technologie

- **C++** / **Raylib** / **Custom UI library**  
- **CSV** voor data opslag  
- **Boid-simulatie** voor visuele zoutrepresentatie  
