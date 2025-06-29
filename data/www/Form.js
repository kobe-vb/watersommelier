
cool = function (data)
{
    console.log(data);

    let name = document.getElementById("name");
    name.innerHTML = data.name;
    
};

window.UserData.onReady(cool);