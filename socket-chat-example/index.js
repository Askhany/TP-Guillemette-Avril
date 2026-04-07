/* 
const express = require('express');
const { createServer } = require('node:http');
const { join } = require('node:path');
const { Server } = require('socket.io');

const app = express();
const server = createServer(app);

// Socket.IO avec récupération d'état de connexion activée
const io = new Server(server, {
  connectionStateRecovery: {
    maxDisconnectionDuration: 2 * 60 * 1000, // 2 minutes
  },
  pingInterval: 25000,
  pingTimeout: 60000,
});

app.get('/', (req, res) => {
  res.sendFile(join(__dirname, 'index.html'));
});

io.on('connection', (socket) => {
  console.log('User connected:', socket.id);

  // Notifier tout le monde qu'un utilisateur s'est connecté
  io.emit('user status', { id: socket.id, status: 'connected' });

  // Recevoir les messages envoyés par un client et les diffuser
  socket.on('chat message', (msg) => {
    io.emit('chat message', msg);
  });

  // Quand un utilisateur se déconnecte
  socket.on('disconnect', () => {
    console.log('User disconnected:', socket.id);
    io.emit('user status', { id: socket.id, status: 'disconnected' });
  });
});

server.listen(3000, () => {
  console.log('Server running at http://localhost:3000');
});

*/

const express = require('express');
const { createServer } = require('node:http');
const { join } = require('node:path');
const { Server } = require('socket.io');

const app = express();
const server = createServer(app);

// Création de l'instance Socket.IO
const io = new Server(server, {
  // Activer la récupération de l'état de connexion
  connectionStateRecovery: {
    maxDisconnectionDuration: 2 * 60 * 1000, // 2 minutes
  },
  pingInterval: 25000,
  pingTimeout: 60000,
});

// Servir la page HTML
app.get('/', (req, res) => {
  res.sendFile(join(__dirname, 'index.html'));
});

// Événement de connexion d'un client
io.on('connection', (socket) => {
  console.log('User connected:', socket.id);

  // Notifier tous les clients qu'un utilisateur s'est connecté
  io.emit('user status', { id: socket.id, status: 'connected' });

  // Écouter les messages envoyés par les clients
  socket.on('chat message', (msg) => {
    io.emit('chat message', msg); // Diffusion à tous
  });

  // Rejoindre une room (facultatif)
  socket.on('join room', (room) => {
    socket.join(room);
    io.to(room).emit('room message', `User ${socket.id} joined room ${room}`);
  });

  // Quitter une room
  socket.on('leave room', (room) => {
    socket.leave(room);
    io.to(room).emit('room message', `User ${socket.id} left room ${room}`);
  });

  // Quand un client se déconnecte
  socket.on('disconnect', () => {
    console.log('User disconnected:', socket.id);
    io.emit('user status', { id: socket.id, status: 'disconnected' });
  });
});

// Démarrer le serveur
server.listen(3000, () => {
  console.log('Server running at http://localhost:3000');
});