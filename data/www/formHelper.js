// Helper functions for dynamic form elements

const FormHelper = {
    
    // Function to add an open-ended question
    addOpenQuestion: function(container, id, label, required = false, placeholder = '') {
        const formGroup = document.createElement('div');
        formGroup.className = 'form-group';
        
        const labelEl = document.createElement('label');
        labelEl.setAttribute('for', id);
        labelEl.textContent = label;
        
        const input = document.createElement('input');
        input.type = 'text';
        input.name = id;
        input.id = id;
        if (required) input.required = true;
        if (placeholder) input.placeholder = placeholder;
        
        formGroup.appendChild(labelEl);
        formGroup.appendChild(input);
        container.appendChild(formGroup);
        
        return formGroup;
    },
    
    // Function to add a textarea question
    addTextareaQuestion: function(container, id, label, required = false, placeholder = '') {
        const formGroup = document.createElement('div');
        formGroup.className = 'form-group';
        
        const labelEl = document.createElement('label');
        labelEl.setAttribute('for', id);
        labelEl.textContent = label;
        
        const textarea = document.createElement('textarea');
        textarea.name = id;
        textarea.id = id;
        textarea.rows = 4;
        if (required) textarea.required = true;
        if (placeholder) textarea.placeholder = placeholder;
        
        formGroup.appendChild(labelEl);
        formGroup.appendChild(textarea);
        container.appendChild(formGroup);
        
        return formGroup;
    },
    
    // Function to add a multiple choice question (radio buttons)
    addMultipleChoiceQuestion: function(container, id, label, options, required = false) {
        const formGroup = document.createElement('div');
        formGroup.className = 'form-group';
        
        const labelEl = document.createElement('legend');
        labelEl.textContent = label;
        
        const fieldset = document.createElement('fieldset');
        fieldset.appendChild(labelEl);
        
        options.forEach((option, index) => {
            const optionContainer = document.createElement('div');
            optionContainer.className = 'radio-option';
            
            const input = document.createElement('input');
            input.type = 'radio';
            input.name = id;
            input.id = `${id}_${index}`;
            input.value = option.value || option;
            if (required && index === 0) input.required = true;
            
            const optionLabel = document.createElement('label');
            optionLabel.setAttribute('for', `${id}_${index}`);
            optionLabel.textContent = option.text || option;
            
            optionContainer.appendChild(input);
            optionContainer.appendChild(optionLabel);
            fieldset.appendChild(optionContainer);
        });
        
        formGroup.appendChild(fieldset);
        container.appendChild(formGroup);
        
        return formGroup;
    },
    
    // Function to add a checkbox question (multiple answers possible)
    addCheckboxQuestion: function(container, id, label, options, required = false) {
        const formGroup = document.createElement('div');
        formGroup.className = 'form-group';
        
        const labelEl = document.createElement('legend');
        labelEl.textContent = label;
        
        const fieldset = document.createElement('fieldset');
        fieldset.appendChild(labelEl);
        
        options.forEach((option, index) => {
            const optionContainer = document.createElement('div');
            optionContainer.className = 'checkbox-option';
            
            const input = document.createElement('input');
            input.type = 'checkbox';
            input.name = `${id}[]`;
            input.id = `${id}_${index}`;
            input.value = option.value || option;
            
            const optionLabel = document.createElement('label');
            optionLabel.setAttribute('for', `${id}_${index}`);
            optionLabel.textContent = option.text || option;
            
            optionContainer.appendChild(input);
            optionContainer.appendChild(optionLabel);
            fieldset.appendChild(optionContainer);
        });
        
        formGroup.appendChild(fieldset);
        container.appendChild(formGroup);
        
        return formGroup;
    },
    
    // Function to add a dropdown/select question
    addSelectQuestion: function(container, id, label, options, required = false, defaultOption = 'Select an option...') {
        const formGroup = document.createElement('div');
        formGroup.className = 'form-group';
        
        const labelEl = document.createElement('label');
        labelEl.setAttribute('for', id);
        labelEl.textContent = label;
        
        const select = document.createElement('select');
        select.name = id;
        select.id = id;
        if (required) select.required = true;
        
        // Default option
        if (defaultOption) {
            const defaultOpt = document.createElement('option');
            defaultOpt.value = '';
            defaultOpt.textContent = defaultOption;
            defaultOpt.disabled = true;
            defaultOpt.selected = true;
            select.appendChild(defaultOpt);
        }
        
        options.forEach(option => {
            const opt = document.createElement('option');
            opt.value = option.value || option;
            opt.textContent = option.text || option;
            select.appendChild(opt);
        });
        
        formGroup.appendChild(labelEl);
        formGroup.appendChild(select);
        container.appendChild(formGroup);
        
        return formGroup;
    },
    
    // Function to add a number input
    addNumberQuestion: function(container, id, label, min = null, max = null, step = 1, required = false) {
        const formGroup = document.createElement('div');
        formGroup.className = 'form-group';
        
        const labelEl = document.createElement('label');
        labelEl.setAttribute('for', id);
        labelEl.textContent = label;
        
        const input = document.createElement('input');
        input.type = 'number';
        input.name = id;
        input.id = id;
        input.step = step;
        if (min !== null) input.min = min;
        if (max !== null) input.max = max;
        if (required) input.required = true;
        
        formGroup.appendChild(labelEl);
        formGroup.appendChild(input);
        container.appendChild(formGroup);
        
        return formGroup;
    },
    
    // Function to add a range slider
    addRangeQuestion: function(container, id, label, min = 0, max = 100, step = 1, defaultValue = 50) {
        const formGroup = document.createElement('div');
        formGroup.className = 'form-group';
        
        const labelEl = document.createElement('label');
        labelEl.setAttribute('for', id);
        labelEl.textContent = label;
        
        const rangeContainer = document.createElement('div');
        rangeContainer.className = 'range-container';
        
        const input = document.createElement('input');
        input.type = 'range';
        input.name = id;
        input.id = id;
        input.min = min;
        input.max = max;
        input.step = step;
        input.value = defaultValue;
        
        const valueDisplay = document.createElement('span');
        valueDisplay.className = 'range-value';
        valueDisplay.textContent = defaultValue;
        
        // Update the value display on change
        input.addEventListener('input', function() {
            valueDisplay.textContent = this.value;
        });
        
        rangeContainer.appendChild(input);
        rangeContainer.appendChild(valueDisplay);
        
        formGroup.appendChild(labelEl);
        formGroup.appendChild(rangeContainer);
        container.appendChild(formGroup);
        
        return formGroup;
    },
    
    // Function to add a date question
    addDateQuestion: function(container, id, label, required = false) {
        const formGroup = document.createElement('div');
        formGroup.className = 'form-group';
        
        const labelEl = document.createElement('label');
        labelEl.setAttribute('for', id);
        labelEl.textContent = label;
        
        const input = document.createElement('input');
        input.type = 'date';
        input.name = id;
        input.id = id;
        if (required) input.required = true;
        
        formGroup.appendChild(labelEl);
        formGroup.appendChild(input);
        container.appendChild(formGroup);
        
        return formGroup;
    },
    
    // Function to add a section header
    addSectionHeader: function(container, title, subtitle = '') {
        const header = document.createElement('div');
        header.className = 'section-header';
        
        const h2 = document.createElement('h2');
        h2.textContent = title;
        header.appendChild(h2);
        
        if (subtitle) {
            const p = document.createElement('p');
            p.textContent = subtitle;
            p.className = 'subtitle';
            header.appendChild(p);
        }
        
        container.appendChild(header);
        return header;
    }
};
