document.getElementById('pricingForm').addEventListener('submit', function(event) {
    event.preventDefault();

    // Gather data from the form
    var formData = {
        method: document.getElementById('method').value,
        type: document.getElementById('type').value,
        stockPrice: document.getElementById('stockPrice').value,
        strikePrice: document.getElementById('strikePrice').value,
        volatility: document.getElementById('volatility').value,
        riskFreeRate: document.getElementById('riskFreeRate').value,
        time: document.getElementById('time').value
    };

    // Construct the API URL based on the method chosen
    var apiUrl = `http://localhost:8081/api/${formData.method}`;

    // Make the API request
    fetch(apiUrl, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(formData)
    })
    .then(response => response.json())
    .then(data => {
        document.getElementById('result').textContent = 'Calculated Option Price: $' + data.price;
        document.getElementById('result').style.display = 'block';
    })
    .catch(error => console.error('Error:', error));
});
