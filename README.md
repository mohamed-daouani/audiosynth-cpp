# Oxono – JavaFX Board Game

**Oxono** is a digital version of the traditional board game, built with **JavaFX** as part of the **DEV3** course at **Haute École Bruxelles-Brabant (HE2B ESI)**.  
It demonstrates object-oriented design, the **MVC** pattern, and GUI development using **FXML**.

---

## Project Overview
- **Course:** DEV3 – Software Development  
- **Institution:** HE2B ESI (Haute École Bruxelles-Brabant)  
- **Author:** Mohamed Daouani  
- **Language:** Java 17+  
- **Framework:** JavaFX  
- **IDE:** IntelliJ IDEA / Scene Builder  

---

## Educational Objectives
This project was created to:
- Apply **object-oriented programming (OOP)** principles  
- Implement an interactive **graphical user interface (GUI)** with **FXML**  
- Follow the **Model–View–Controller (MVC)** architecture  
- Manage **user events and game logic** effectively  

---

## Game Description
**Oxono** is a turn-based logic and strategy game where two players alternately place their marks on a grid, aiming to align symbols or block the opponent.  

The application provides:
- Turn-based gameplay  
- Automatic win condition detection  
- Game reset and replay options  
- An intuitive and responsive **JavaFX interface**  

---

## Architecture
The project follows an **MVC** architecture for clarity and modularity:

```
src/
├── model/
│   ├── Board.java
│   ├── Cell.java
│   ├── Player.java
│   └── GameLogic.java
├── view/
│   ├── oxono.fxml
│   └── assets/
├── controller/
│   └── OxonoController.java
└── Main.java
```

---

## ⚙️ Run the Project
1. Clone the repository  
   ```bash
   git clone https://github.com/mohameddaouani/oxono-javafx.git
   cd oxono-javafx
   ```
2. Open it in **IntelliJ IDEA**  
3. Configure **JavaFX** libraries if needed  
4. Run the `Main` class  

---

## Key Features
- Clean and modular **MVC** structure  
- Interactive **JavaFX interface** with **FXML**  
- **Turn-based gameplay** and win detection  
- **Reset** and **replay** functionality  
- Well-structured and readable source code  

---

> © 2025 – Oxono, developed by **Mohamed Daouani** for the **DEV3 course** at **HE2B ESI**.
