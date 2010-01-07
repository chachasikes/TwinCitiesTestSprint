// $Id: admin.menu.js,v 1.1.2.1 2009/11/20 02:44:32 yhahn Exp $

Drupal.behaviors.adminToolbarMenu = function(context) {
  $('#admin-toolbar div.admin-block:has(ul.menu):not(.admin-toolbar-menu)').each(function() {
    var menu = $(this);
    menu.addClass('admin-toolbar-menu');
    if ($('ul.menu', menu).size() > 0) {
      Drupal.adminToolbarMenu.init(menu);
    }
  });
};

Drupal.adminToolbarMenu = {
  'init': function (menu) {
    // Attach click handlers to menu items
    $('ul.menu li:not(.leaf)', menu).click(function() {
      if ($(this).parent().is('.admin-active-menu')) {
        if (menu.data('disableMenu')) {
          return true;
        }
        else {
          var url = $(this).children('a').attr('href');
          var active_link = $('ul.menu a[href='+url+']', menu);
          Drupal.adminToolbarMenu.setActive(menu, active_link);
          return false;
        }
      }
    });
    $('ul.menu li:not(.leaf) a', menu).click(function() {
      menu.data('disableMenu', true);
    });

    // Set initial active menu state.
    var active_link;
    if (Drupal.settings.activePath) {
      if ($('ul.menu a[href='+Drupal.settings.activePath+']', menu).size() > 0) {
        active_link = $('ul.menu a[href='+Drupal.settings.activePath+']', menu).addClass('active');
      }
    }
    if (!active_link) {
      active_link = $('ul.menu a.active', menu).size() ? $('ul.menu a.active', menu) : $('ul.menu > li > a', menu);
    }
    if (active_link) {
      Drupal.adminToolbarMenu.setActive(menu, $(active_link[0]));
    }
  },

  'setActive': function(menu, active_link) {
    var breadcrumb = [];
    var active_menu;

    $(active_link).each(function() {
      // Traverse backwards through menu parents and build breadcrumb array.
      $(this).parents('ul.menu').each(function() {
        $(this).siblings('a').each(function() {
          breadcrumb.unshift($(this));
        });
      });

      // If we have a child menu (actually a sibling in the DOM), use it
      // as the active menu. Otherwise treat our direct parent as the
      // active menu.
      if ($(this).siblings('ul.menu').size()) {
        active_menu = $(this).siblings('ul.menu');
        breadcrumb.push($(this));
      }
      else {
        active_menu = $(this).parents('ul.menu');
      }
      if (active_menu) {
        $('ul.menu', menu).removeClass('admin-active-menu').removeClass('clear-block');
        $(active_menu[0]).addClass('admin-active-menu').addClass('clear-block').parents('li').show();
      }
    });

    // Render the breadcrumb to the admin tab
    if (breadcrumb.length > 0) {
      var bid = $(menu).attr('id').split('block-')[1];
      var tab = $('#admin-toolbar div.admin-tab.'+bid+' span');
      tab.empty();
      for (var key in breadcrumb) {
        $(breadcrumb[key]).clone().appendTo(tab);
      }
      $('a', tab).click(function() {
        if ($(this).parents('div.admin-tab').is('.admin-tab-active')) {
          var url = $(this).attr('href');
          var active_link = $('ul.menu a[href='+url+']', menu);
          Drupal.adminToolbarMenu.setActive(menu, active_link);
        }
        else {
          $(this).parents('div.admin-tab').click();
        }
        return false;
      });
    }

    return false;
  }
};
