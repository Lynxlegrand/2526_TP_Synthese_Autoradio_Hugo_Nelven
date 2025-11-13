# 🎓 TP de Synthèse — Autoradio

## 👥 Équipe

| Nom | Prénom | Groupe |
|:--|:--|:--:|
| THÉBAULT | [Nelven](https://github.com/NelvTheb) | ESE TP1 |
| CORDI | [Hugo](https://github.com/Lynxlegrand) | ESE TP1 |

📅 **Séances de TP :** 
- séance 1 : 13 novembre 2025 
- séance 2 :
- séance 3 :
- séance 4 :

🏫 **ENSEA — 3A ESE**  
👨‍🏫 **Encadrant :** [M.Fiack](https://github.com/lfiack)  

---

## 🎯 Objectifs du TP

> **But :** Concevoir et analyser un système d’autoradio analogique simplifié comprenant :
>
> - La réception et la démodulation du signal audio  
> - L’amplification du signal de sortie  
> - La gestion de la commande de volume  
>
> Ce TP vise à mettre en œuvre les compétences acquises en électronique analogique et en traitement du signal.

---

## 🧩 Schéma fonctionnel

```mermaid
graph LR
A[Signal RF reçu] --> B[Démodulateur AM/FM]
B --> C[Filtrage passe-bande]
C --> D[Amplificateur audio]
D --> E[Haut-parleur]
subgraph Alimentation
F[+12V véhicule]
end
F --> D