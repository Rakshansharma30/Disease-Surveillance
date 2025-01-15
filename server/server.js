const express = require('express');
const path = require('path');
const app = express();
const PORT = 3000;

// Serve static files from the client folder
app.use(express.static(path.join(__dirname, '../client')));

// Endpoint to test model availability
app.get('/test-model', (req, res) => {
    res.json({ message: "Model is available" });
});

// Endpoint to handle predictions
app.post('/predict', express.json(), (req, res) => {
    const symptoms = req.body;
    console.log("Received symptoms:", symptoms);

    // Simulate model prediction
    const probability = Math.random(); // Simulated prediction probability
    const prediction = probability > 0.8 ? "High Risk"
        : probability > 0.5 ? "Moderate Risk"
        : "Low Risk";

    res.json({
        prediction,
        probability,
        details: ""
    });
});

// Start the server
app.listen(PORT, () => {
    console.log(`Server running on http://localhost:${PORT}`);
});
