
import random
import config  

# definiendo estado de jugadores

class Player:
    def __init__(self, name, writer):
        self.name = name
        self.writer = writer  # El objeto socket para enviarle mensajes
        self.team = None      # Se asignará al unirse a un equipo
        
        # Nuevas estadísticas de combate
        self.hp = config.PLAYER_MAX_HP
        self.is_defeated = False
        self.is_defending = False
    def take_damage(self, amount):
        """Resta HP al jugador y comprueba si es derrotado."""
        final_damage = amount

        defense_msg = ""
        if self.is_defending:
            final_damage = amount//2  # Defensa reduce el daño en 0.5
            defense_msg = f" (pero {self.name} se puso durito y bloquea la mitad del daño, DAÑO FINAL: {final_damage})"
            self.is_defending = False  # Defensa solo dura un ataque
       
        self.hp -= final_damage
        defeat_msg = None
        
        if self.hp <= 0:
            self.hp = 0
            self.is_defeated = True
            defeat_msg = f"¡{self.name} ha sido derrotado!"
        
        damage_msg = f"HP restante de {self.name}: {self.hp}"
        return damage_msg + defense_msg, defeat_msg


# definiendo estado de equipos

class Team:
    def __init__(self, name):
        self.name = name
        self.players = []

    def add_player(self, player):
        self.players.append(player)
        player.team = self

    def is_defeated(self):
        """Comprueba si todos los jugadores del equipo están derrotados."""
        for player in self.players:
            if not player.is_defeated:
                return False  # Al menos uno sigue vivo
        return True  # Todos están derrotados

# gamestate:
# Gestiona el estado y las reglas del juego.
# NO sabe nada de sockets, solo aplica la lógica.

class GameState:
    def __init__(self):

        self.teams = {}                # {"NombreEquipo": ObjetoTeam}
        self.players_by_writer = {}    # {writer: ObjetoPlayer}
        self.players_by_name = {}      # {"nombrejugador": ObjetoPlayer}
        self.game_started = False
        self.turn_order = []           # Lista ordenada de ObjetosPlayer
        self.current_turn_index = 0

    def add_player(self, player, team_name):
        """Añade un nuevo jugador al juego y a un equipo."""
        
        # Crear equipo si no existe
        if team_name not in self.teams:
            if len(self.teams) >= config.MAX_TEAMS:
                return False, "No se pueden crear más equipos (límite alcanzado)."
            self.teams[team_name] = Team(team_name)
            

        team = self.teams[team_name]

        # Validar límites y nombres
        if len(team.players) >= config.MAX_PLAYERS_PER_TEAM:
            return False, f"El equipo {team_name} está lleno."
            
        if player.name.lower() in self.players_by_name:
            return False, f"El nombre '{player.name}' ya está en uso."

        # Añadir jugador
        team.add_player(player)
        self.players_by_writer[player.writer] = player
        self.players_by_name[player.name.lower()] = player
        
        return True, f"Jugador {player.name} se unió al {team_name}."

    def check_start_condition(self):
        """Comprueba si se cumplen las condiciones para iniciar el juego."""
        if self.game_started: return False
        if len(self.teams) < config.MIN_TEAMS: return False

        # Comprobar que todos los equipos tengan el mínimo de jugadores
        for team in self.teams.values():
            if len(team.players) < config.MIN_PLAYERS_PER_TEAM:
                return False
        
        # se cumple todo
        self.game_started = True
        self.setup_turns()
        return True

    def setup_turns(self):
        """Crea la lista de orden de turnos, intercalando equipos."""
        interleaved_players = []
        max_players = max(len(t.players) for t in self.teams.values())
        team_list = list(self.teams.values())
        
        for i in range(max_players):
            for team in team_list:
                if i < len(team.players):
                    interleaved_players.append(team.players[i])
        
        self.turn_order = interleaved_players
        self.current_turn_index = 0

    def get_current_player(self):
        """Devuelve el ObjetoPlayer del jugador en turno."""
        return self.turn_order[self.current_turn_index]

    def next_turn(self):
        """Avanza al siguiente jugador *que no esté derrotado*."""
        max_attempts = len(self.turn_order)  # Para evitar bucles infinitos
        
        for _ in range(max_attempts):
            self.current_turn_index = (self.current_turn_index + 1) % len(self.turn_order)
            next_player = self.get_current_player()
            if not next_player.is_defeated:
                return  # Encontramos al siguiente jugador válido
        
        # Si este bucle termina, significa que no hay más jugadores activos.
        # La lógica de 'check_win_condition' debería haber terminado el juego antes.
        print("ADVERTENCIA: next_turn() no encontró jugadores activos.")

    def check_win_condition(self):
        """Revisa si solo queda un equipo activo."""
        active_teams = []
        for team in self.teams.values():
            if not team.is_defeated():
                active_teams.append(team)

        if len(active_teams) == 1:
            return active_teams[0]  # Devuelve el equipo ganador
        return None  # El juego continúa (o es un empate si active_teams == 0)

    def get_status_summary(self):
        """Genera un string con el estado de HP/Pos de todos."""
        summary = ["--- Estado del Tablero ---"]
        for team in self.teams.values():
            summary.append(f"Equipo {team.name}:")
            for player in team.players:
                status = "DERROTADO" if player.is_defeated else f"HP: {player.hp}/{config.PLAYER_MAX_HP}"
                summary.append(f"  > {player.name.ljust(15)} [{status}]")
        return "\n".join(summary)
        
    # acciones
    
    def process_action(self, player, command_str):
        """Punto de entrada principal. Recibe un comando del server y lo procesa."""
        
        if player.is_defending:
            player.is_defending = False  # Defensa solo dura un turno

        # Validaciones de Turno y Estado
        if player != self.get_current_player():
            return "Espera", "No es tu turno."
        if player.is_defeated:
            return "Espera", "Estás derrotado. No puedes realizar acciones."

        # Parsear Comando
        parts = command_str.lower().strip().split()
        if not parts:
            return "Error", "Comando vacío."
        
        action = parts[0]
        status, message = "Error", "Comando no reconocido." # Default

        # Procesar Acción
        
        if action == "atacar":
            if len(parts) < 2:
                status, message = "Error", "Debes especificar un objetivo. Ej: atacar <nombre>"
            else:
                target_name = parts[1]
                status, message = self._process_attack(player, target_name)
        # si defiende
        elif action == "defender":
            status, message = self._process_defend(player)
        
        elif action == "estado":
            # Una acción útil para que el jugador vea el tablero
            return "Info", self.get_status_summary()

        else:
             return "Error", f"Comando '{action}' no reconocido. Usa 'atacar <nombre>' o 'estado'."

        # Post-Acción (si fue exitosa)
        if status == "Accion":
            # Si la acción fue válida, comprobar si alguien ganó
            winner_team = self.check_win_condition()
            if winner_team:
                # Juego terminado!
                return "Ganador", winner_team.name
            
            # Si no hay ganador, pasa al siguiente turno
            self.next_turn()
        
        return status, message  # Devuelve el resultado de la acción

    def _process_attack(self, player, target_name):
        """Lógica interna para la acción 'atacar'."""
        
        # Buscar objetivo
        target = self.players_by_name.get(target_name.lower())
        
        if not target:
            return "Error", f"No se encontró al jugador '{target_name}'."
        if target.is_defeated:
            return "Error", f"{target.name} ya está derrotado."
        if target.team == player.team:
            return "Error", "No puedes atacar a un compañero de equipo."
        # Ejecutar ataque
        roll = random.randint(1, config.DIE_SIDES_R) # El dado decide el daño
        damage_msg, defeat_msg = target.take_damage(roll)
        
        full_msg = f"¡{player.name} ataca a {target.name} por {roll} de daño! {damage_msg}"
        if defeat_msg:
            full_msg += f"\n!!! {defeat_msg} !!!"
            
        return "Accion", full_msg
    def _process_defend(self, player):
        """Lógica interna para la acción 'defender'."""
        player.is_defending = True
        return "Accion", f"¡{player.name} usa endurecer!"