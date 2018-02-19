<?php

function bld_client_services_admin_form($form, &$form_state) {
  $form['description'] = [
    '#markup' => '<p>' . t('In this configuration you will have information regarding the import of contents.') . '</p>',
  ];
  $form['run'] = [
    '#type' => 'submit',
    '#value' => t('Run import'),
    '#submit' => ['bld_client_services_run_import_submit'],
  ];
  $status = '<p>' . t('Last import: %cron-last ago.', ['%cron-last' => format_interval(REQUEST_TIME - variable_get('cron_import_last')),]) . '</p>';
  $form['status'] = [
    '#markup' => $status,
  ];


  $form['bld_import'] = [
    '#type' => 'fieldset',
  ];
  $form['bld_import']['import_safe_threshold'] = [
    '#type' => 'select',
    '#title' => t('Run import every'),
    '#description' => t('Select the time interval to perform the import.'),
    '#default_value' => variable_get('cron_safe_threshold', DRUPAL_CRON_DEFAULT_THRESHOLD),
    '#options' => [0 => t('Never')] + drupal_map_assoc([
        3600,
        10800,
        21600,
        43200,
        86400,
        604800,
      ], 'format_interval'),
  ];


  //return $form;
  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = [
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  ];

  if (!empty($_POST) && form_get_errors()) {
    drupal_set_message(t('The settings have not been saved because of the errors.'), 'error');
  }
  //@TODO Implement submit function
  $form['#submit'][] = 'bld_client_services_settings_form_submit';
  // By default, render the form using theme_system_settings_form().
  if (!isset($form['#theme'])) {
    $form['#theme'] = 'system_settings_form';
  }
  return $form;
}

function bld_client_services_run_import_submit($form, &$form_state) {
  global $user;
  bld_client_services_batch_1();
  variable_set('cron_import_last', REQUEST_TIME);
  watchdog('bld_client_services', t('Import contents by services run completed.'));
}

function bld_client_services_settings_form_submit($form, &$form_state) {
  variable_set('cron_safe_threshold', $form_state['values']['import_safe_threshold']);
  drupal_set_message(t('The configuration options have been saved.'));
}

function bld_client_services_settings_form($form, &$form_state) {
  $client = new DrupalBldClient();

  $form['container_base'] = [
    '#type' => 'details',
    '#title' => t('Endpoint base'),
    '#weight' => -10,
    '#open' => TRUE,
  ];

  //child of this container will be inline.
  $form['container_base']['base_container'] = [
    '#type' => 'container',
    '#attributes' => [
      'class' => ['container-inline'],
    ],
  ];

  $form['container_base']['base_container']['protocol'] = [
    '#type' => 'select',
    //'#options' => array('value', 'value1'),
    '#options' => $client->getProtocols(),
    '#default_value' => variable_get('bld.services_client.protocol', 'HTTP'),
  ];
  $form['container_base']['base_container']['markup_url_base'] = [
    '#markup' => '<div>' . t('://') . '</div>',
  ];

  $form['container_base']['base_container']['endpoint_base'] = [
    '#type' => 'textfield',
    '#size' => 25,
    '#required' => TRUE,
    '#default_value' => variable_get('bld.services_client.url', ''),
    //'#description' => t('Endpoint URL example "66.171.255.246:7468/NewsService.svc".'),
  ];
  $form['container_base']['base_container']['markup_backslash'] = [
    '#markup' =>  t('/All/All/'),
  ];
  $form['container_base']['base_container']['endpoint_base_key'] = [
    '#type' => 'textfield',
    '#size' => 10,
    '#required' => TRUE,
    '#default_value' => variable_get('bld.services_client.key', ''),
    //'#description' => t('Endpoint URL example "66.171.255.246:7468/NewsService.svc".'),
  ];
  $form['container_base']['base_container']['markup_more_backslash'] = [
    '#markup' =>  t('?page=1&size=10'),
  ];

  //return $form;
  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = [
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  ];
  $form['#submit'][] = 'bld_client_services_admin_form_settings_form_submit';
  // By default, render the form using theme_system_settings_form().
  if (!isset($form['#theme'])) {
    $form['#theme'] = 'system_settings_form';
  }

  return $form;
}

function bld_client_services_admin_form_settings_form_submit($form, &$form_state) {
  variable_set('bld.services_client.protocol', $form_state['values']['protocol']);
  variable_set('bld.services_client.url', $form_state['values']['endpoint_base']);
  variable_set('bld.services_client.key', $form_state['values']['endpoint_base_key']);
  drupal_set_message(t('The configuration options have been saved.'));
}
