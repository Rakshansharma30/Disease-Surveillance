const VALID_SYMPTOMS = [
    "fever", "diarrhea", "muscleweakness", "musclepain",
    "chills", "headache", "nausea", "pain",
    "thickenedskin", "hairthinning", "vomiting", "weightloss",
    "weakness", "anorexia", "lethargy"
];

function collectSymptoms() {
    const symptomInputs = document.getElementById("symptomInputs");
    symptomInputs.innerHTML = "";

    VALID_SYMPTOMS.forEach((symptom, index) => {
        const symptomInput = document.createElement("div");
        symptomInput.className = "symptom-input";
        symptomInput.innerHTML = `
            <label>${symptom.charAt(0).toUpperCase() + symptom.slice(1)}:</label>
            <select name="symptom${index}" required>
                <option value="0">None (0)</option>
                <option value="0.33">Mild (1)</option>
                <option value="0.67">Moderate (2)</option>
                <option value="1">Severe (3)</option>
            </select>
        `;
        symptomInputs.appendChild(symptomInput);
    });

    document.getElementById("symptomForm").style.display = "block";
}

async function testModelAvailability() {
    try {
        const response = await fetch("http://localhost:3000/test-model");
        return await response.json();
    } catch (error) {
        console.error("Model test failed:", error);
        return null;
    }
}

async function submitSymptoms(event) {
    event.preventDefault();

    const resultElement = document.getElementById("result");
    resultElement.innerHTML = "<div class='loading'>Checking system status...</div>";

    // Test model availability
    const modelStatus = await testModelAvailability();
    if (!modelStatus || modelStatus.error) {
        resultElement.innerHTML = `
            <div class="error">
                System Error: Disease prediction model not available.<br>
                Please contact system administrator.<br>
                Details: ${modelStatus ? modelStatus.error : 'Could not connect to server.'}
            </div>
        `;
        return;
    }

    const form = document.getElementById("symptomForm");
    const formData = new FormData(form);
    const data = {};

    formData.forEach((value, key) => {
        data[key] = value;
    });

    resultElement.innerHTML = "<div class='loading'>Analyzing symptoms...</div>";

    try {
        const response = await fetch("http://localhost:3000/predict", {
            method: "POST",
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(data)
        });

        if (!response.ok) {
            const errorData = await response.json();
            throw new Error(errorData.error || 'Server error');
        }

        const responseData = await response.json();
        resultElement.innerHTML = `
            <div class="prediction-result">
                <h2>${responseData.prediction} Risk</h2>
                <p>Probability: ${(responseData.probability * 100).toFixed(1)}%</p>
                <p>${responseData.details}</p>
            </div>
        `;
    } catch (error) {
        resultElement.innerHTML = `
            <div class="error">
                Error: ${error.message || 'An error occurred during prediction.'}<br>
                Please try again or contact support if the issue persists.
            </div>
        `;
    }
}
