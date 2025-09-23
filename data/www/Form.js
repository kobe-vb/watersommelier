cool = function (data) {
    console.log(data);

    let name = document.getElementById("name");
    name.innerHTML = data.name;
    
    const form = document.querySelector('form');
    const submitButton = form.querySelector('button[type="submit"]').parentNode;
    
    // Add custom questions
    FormHelper.addOpenQuestion(form, 'favorite_water', 'What is your favorite water?', true);
    FormHelper.addMultipleChoiceQuestion(
        form, 
        'water_preference', 
        'What type of water do you prefer?',
        ['Tap water', 'Spring water', 'Mineral water', 'Filtered water'],
        true
    );
    
    // afblijven!!!!
    form.insertBefore(form.removeChild(submitButton), null);
};


window.UserData.onReady(cool);